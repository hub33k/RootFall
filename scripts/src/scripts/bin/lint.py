#!/usr/bin/env python3

import subprocess
import sys
from pathlib import Path

from scripts.utils import ROOT_DIR

# Configuration
SOURCE_DIR = ROOT_DIR
BUILD_DIR = SOURCE_DIR / "build-debug"
EXTENSIONS = {".cpp", ".cxx", ".cc", ".h", ".hpp"}
EXCLUDE_DIRS = {
    "third_party",
    "external",
    "build",
    "build-vendor",
    ".venv",
    ".cache",
    ".idea",
    ".git",
    "hub33k-notes",
}


def is_source_file(path: Path):
    return path.suffix in EXTENSIONS and not any(
        part in EXCLUDE_DIRS for part in path.parts
    )


def find_source_files():
    return [p for p in SOURCE_DIR.rglob("*") if is_source_file(p)]


def run_clang_tidy(file_path: Path):
    cmd = ["clang-tidy", str(file_path), "-p", str(BUILD_DIR)]
    print(f"🔍 Linting: {file_path}")
    try:
        result = subprocess.run(cmd, check=False, capture_output=True, text=True)
        if result.stdout:
            print(result.stdout)
        if result.stderr:
            print(result.stderr, file=sys.stderr)
        return result.returncode
    except FileNotFoundError:
        print(
            "❌ clang-tidy not found. Install it and ensure it's in your PATH.",
            file=sys.stderr,
        )
        sys.exit(1)


def main():
    print("not working as expected, skipping...")
    sys.exit(0)

    try:
        files = find_source_files()
        if not files:
            print("No source files found.")
            return

        total_errors = 0
        for f in files:
            code = run_clang_tidy(f)
            if code != 0:
                total_errors += 1

        if total_errors > 0:
            print(f"\n❗ clang-tidy found issues in {total_errors} file(s).")
            sys.exit(1)
        else:
            print("\n✅ All files passed clang-tidy checks.")

    except KeyboardInterrupt:
        print("\n❌ Linting interrupted by user.", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"❌ An error occurred: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()

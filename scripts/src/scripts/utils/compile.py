from pathlib import Path
import shlex
import subprocess
import sys

CC = "clang++"
LINKER = "clang++"


def compile(
    filename: str,
    filepath: Path | str,
    output: Path | str,
    cwd: Path | str = Path.cwd(),
    flags: str = "",
    intermediate: bool = False,
    verbose: bool = False,
):
    """Compile a C++ source file to object file."""
    if verbose:
        print(f"🔧 Compiling {filepath} -> {output}")

    src_file = Path(filepath).as_posix()
    obj_file = Path(output).as_posix()

    if not src_file.exists():
        print(f"❌ Source file {src_file} does not exist.")
        return

    # if intermediate:
    # obj_file = Path(BUILD_DIR / f"{filename}.i").as_posix()

    try:
        command = f"{CC} -c {str(src_file)} -o {str(obj_file)} {flags}"
        subprocess.run(
            shlex.split(command),
            cwd=cwd,
        )
    except Exception as e:  # noqa: F841
        print("Compile error.")
        sys.exit(1)


def link():
    pass

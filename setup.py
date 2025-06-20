import re
import subprocess
from rich.console import Console
from rich.table import Table
from shutil import which
from typing import TypedDict, Dict
from packaging import version

console = Console()

# TODO (hub33k): if there is a missing dependency, install it


class Dependency(TypedDict):
    version: str
    """
    Minimum required version of a dependency.
    """


required_dependencies: Dict[str, Dependency] = {
    "uv": {
        "version": "0.6",
    },
    "python": {
        "version": "3.13",
    },
    "git": {
        "version": "2.49",
    },
    "cmake": {
        "version": "3.31",
    },
    "clang": {
        "version": "20",
    },
    "ninja": {
        "version": "1.12",
    },
}

missing_dependencies = {}


def clean_version_string(ver: str) -> str:
    # Extract only the numeric (semver-like) part at the beginning
    match = re.search(r"\d+(\.\d+)*", ver)
    return match.group(0) if match else "0.0.0"


def get_installed_version(cmd: str, flag: str = "--version") -> str:
    try:
        result = subprocess.check_output(
            [cmd, flag], stderr=subprocess.STDOUT, text=True
        )
        for part in result.strip().split():
            if any(char.isdigit() for char in part):
                return part.strip("v")
    except Exception as e:
        return f"Error: {e}"
    return "0.0.0"


def check_version_matches(program: str) -> tuple[bool, str]:
    expected_version = version.parse(required_dependencies[program]["version"])
    actual_raw = get_installed_version(program)

    if actual_raw.startswith("Error"):
        return False, actual_raw

    actual_clean = clean_version_string(actual_raw)
    actual = version.parse(actual_clean)

    # print(f"{program}: required >= {expected}, found = {actual}")
    return actual >= expected_version, actual_clean


# check if dependencies are installed and version matches
def check_dependencies() -> None:
    table = Table(title="Dependency Check", show_lines=True)
    table.add_column("Program", style="bold cyan")
    table.add_column("Installed", justify="center")
    table.add_column("Version Match", justify="center")
    table.add_column("Minimum Required Version", justify="center")
    table.add_column("Current Version", justify="center")

    for program, dep in required_dependencies.items():
        required_ver = dep["version"]
        is_installed = which(program) is not None

        if not is_installed:
            missing_dependencies[program] = dep
            table.add_row(
                program,
                "[red]❌[/red]",
                "[red]N/A[/red]",
                required_ver,
                "[red]Not found[/red]",
            )
            continue

        matches, current_ver = check_version_matches(program)

        if not matches:
            missing_dependencies[program] = dep

        table.add_row(
            program,
            "[green]✅[/green]",
            "[green]✓[/green]" if matches else "[red]❌[/red]",
            required_ver,
            current_ver,
        )

    console.print(table)


def main():
    check_dependencies()

    if len(missing_dependencies) == 0:
        console.print(
            "\n[bold green]All dependencies are installed and up to date![/bold green]"
        )
    else:
        console.print(
            "\n[bold yellow]Some dependencies are missing or outdated.[/bold yellow]"
        )

        print()

        for program, dep in missing_dependencies.items():
            console.print(f"Missing {program} version {dep['version']}")


if __name__ == "__main__":
    main()

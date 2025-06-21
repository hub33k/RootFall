from pathlib import Path
import pathlib
import shlex
import platform
import subprocess

# Project root dir
ROOT_DIR = Path(__file__).parents[4]

is_windows = platform.system() == "Windows"
is_macos = platform.system() == "Darwin"
is_linux = platform.system() == "Linux"
is_unix = is_linux or is_macos


def exec(
    command,
    cwd=ROOT_DIR,
    shell=False,
    description=None,
    verbose=True,
    **kwargs,
) -> None | subprocess.CompletedProcess:
    try:
        if verbose:
            print("[Command]", command)

        if description:
            print(f"[desc] {description}")

        result = subprocess.run(
            shlex.split(command),
            cwd=cwd,
            shell=shell,
            **kwargs,
        )
        return result
    except Exception as e:
        print("Error: %s - %s." % (e.filename, e.strerror))


def normalize_local_path(path: pathlib.Path, root_dir=ROOT_DIR) -> str:
    return (
        str(path)
        .replace(str(root_dir) + str(pathlib.os.sep), "")
        .replace(str(pathlib.os.sep), "/")
    )

import os
import shutil
import pathlib


def __remove_all_extracted_folders():
    for x in os.listdir():
        if not os.path.isdir(x):
            continue
        if not x.startswith("utf-8-paths"):
            continue
        shutil.rmtree(x)


def __try_renaming(original: str, new_name: str):
    if os.path.isdir(original) or os.path.isfile(original):
        os.rename(original, new_name)


def main():
    __remove_all_extracted_folders()

    repo_root_dir = pathlib.Path(__file__).parent.parent.resolve()
    test_tar_path = repo_root_dir / "data" / "utf-8-paths.tar.gz"

    os.system(f"cmake -E tar xv \"{test_tar_path}\"")
    __try_renaming("utf-8-paths", "utf-8-paths-global-cmake")

    os.system(f"tar -xvf \"{test_tar_path}\"")
    __try_renaming("utf-8-paths", "utf-8-paths-global-tar")

    os.system(f"bsdtar -xvf \"{test_tar_path}\"")
    __try_renaming("utf-8-paths", "utf-8-paths-global-bsdtar")


if __name__ == "__main__":
    main()

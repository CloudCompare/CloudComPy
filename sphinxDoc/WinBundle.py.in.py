import subprocess
from pathlib import Path
import shutil
import os
import re

class winBundle:
    """
    Class to collect all the DLL dependencies of a Windows application and copy them to the application directory.
    The process is as follows:
    1. Collect initial binaries (exe, dll, pyd) from the application directory.
    2. For each binary, get direct dependencies using dumpbin.
    3. Recursively find dependencies to add to the Bundle.
    4. copy all the dependencies found to the application directory.
    """
    def __init__(self, app_dir):
        print("=== Initializing winBundle with app_dir: " + str(app_dir) + "===")
        self.appDir = app_dir
        self.initialList = set()
        self.toAnalyze = set()
        self.analyzed = set()
        self.toCopy = set()
        self.filteredPath = self.filtered_path()
        self.toDiscard = ['icuuc.dll'] # DLL à ignorer, souvent présentes dans PATH mais pas à redistribuer
        #, 'icuin.dll', 'icudt.dll', 'icuin.dll', 'vcruntime140.dll', 'msvcp140.dll'

    def filtered_path(self):
        """
        Filter PATH to avoid system directories and development tools that could interfere.
        We only want to keep dll that could be used by our binaries, not system or dev dll that won't be redistributed.
        """
        keep = []
        print("Original PATH: " + os.environ["PATH"])
        for p in os.environ["PATH"].split(os.pathsep):
            p = Path(p)
            pl = str(p).lower()

            # Exclusions système
            if pl.startswith(r"c:\windows"):
                continue
            if pl.startswith(r"c:\program files"):
                continue
            if pl.startswith(r"c:\programdata"):
                continue
            if "nvidia" in pl:
                continue
            if "windowsapps" in pl:
                continue
            if "windows kits" in pl:
                continue
            if "git" in pl:
                continue
            if "powershell" in pl:
                continue
            if "dotnet" in pl:
                continue

            keep.append(p)
        print("filtered path: " + str(keep))
        return keep

    def get_direct_dependencies(self, binary):
        """
        Get direct dependencies of a binary (exe or dll) using dumpbin.
        Only DLL names are extracted, not paths.
        Dependencies paths are solved after that, using resolve_dll.
        """
        cmd = ["dumpbin", "/DEPENDENTS", str(binary)]
        output = subprocess.check_output(cmd, text=True, errors="ignore")

        deps = []
        for line in output.splitlines():
            m = re.search(r"\s+([A-Za-z0-9_.-]+\.dll)", line, re.IGNORECASE)
            if m:
                deps.append(m.group(1))
        #print("Direct dependencies for " + str(binary) + ": " + ", ".join(deps))
        return deps

    def resolve_dll(self, name, extra_dirs=None):
        """
        Solve a DLL by searching in:
        1. Application directory
        2. extra_dirs (Qt, Conda, staging…)
        3. Filtered PATH
        The full path of the DLL is returned if found, resolving symbolic links, otherwise None.
        """
        # 1. Application directory
        candidate = self.appDir / name
        if candidate.exists():
            return candidate.resolve()

        # 2. Extra dirs
        if extra_dirs:
            for d in extra_dirs:
                candidate = Path(d) / name
                if candidate.exists():
                    return candidate.resolve()

        # 3. PATH filtré
        for p in self.filteredPath:
            candidate = p / name
            if candidate.exists():
                return candidate.resolve()

        return None
    
    def collect_initial_binaries(self):
        """
        Collect initial binaries from main executable, Python libs and plugins.
        """
        print("=== Collect initial binaries ===")
        for root, _, files in os.walk(self.appDir):
            for name in files:
                ext = Path(name).suffix
                if ext in (".dll", ".pyd", ".exe"):
                    library = self.appDir / root / name
                    self.initialList.add(name)
                    self.toAnalyze.add(library)
        print("Initial binaries collected: " + ", ".join(str(b) for b in self.initialList))

    def find_all_dependencies(self):
        """
        Find all the dependencies using initial binaries and recursive solving.
        Only DLL with resolved paths are added to the list to be copied to the bundle.
        """
        print("=== Find all dependencies ===")
        while self.toAnalyze:
            binary = self.toAnalyze.pop()
            if binary in self.analyzed:
                continue

            deps = self.get_direct_dependencies(binary)
            for dep in deps:
                resolved = self.resolve_dll(dep)
                #print("Resolving " + dep + ": " + (str(resolved) if resolved else "NOT FOUND in FILTERED PATH"))
                if resolved and (resolved not in self.analyzed):
                    self.toAnalyze.add(resolved)
                    if dep not in self.initialList and dep not in self.toDiscard:
                        self.toCopy.add(resolved)

            self.analyzed.add(binary)

    def copy_dll(self):
        print("=== copy DLL found ===")
        for d in sorted(self.toCopy):
            print(d)
            shutil.copy2(d, self.appDir)


if __name__ == "__main__":

    print(" === Fill Bundle with required DLL ===")
    app_dir = Path(r"@CMAKE_INSTALL_PREFIX@/cloudComPy")
    bundle = winBundle(app_dir)
    bundle.collect_initial_binaries()
    bundle.find_all_dependencies()
    bundle.copy_dll()
    print(" === Bundle done ===")

    
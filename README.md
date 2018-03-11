# Pacman
EFI Shell Pacman game 

Simple EFI Shell applications that uses gop driver/EFI to play pacman clone


Compilation:
build -p AppPkg/AppPkg.dsc
Add this folder to AppPkg/Applications/Pacman


Edit AppPkg.dsc with the following change:
+  BmpSupportLib|MdeModulePkg/Library/BaseBmpSupportLib/BaseBmpSupportLib.inf
+  SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
+  AppPkg/Applications/Pacman/Pacman.inf        # No LibC includes or functions.

![](https://i.imgur.com/tjO3bPf.png "screenshot")


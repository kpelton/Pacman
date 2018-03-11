# Pacman
EFI Shell Pacman game 

Simple EFI Shell applications that uses gop driver/EFI to play pacman clone
![alt tag](https://imgur.com/a/eL3qG "")


COMPILATION:
Build command is:build -p AppPkg/AppPkg.dsc
Add this folder to AppPkg/Applications/Pacman
Edit AppPkg.dsc with the following change:
diff --git a/AppPkg/AppPkg.dsc b/AppPkg/AppPkg.dsc
index 58bc84e..f5284fe 100644
--- a/AppPkg/AppPkg.dsc
+++ b/AppPkg/AppPkg.dsc
@@ -80,7 +80,8 @@
   HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
   FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
   SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
-
+  BmpSupportLib|MdeModulePkg/Library/BaseBmpSupportLib/BaseBmpSupportLib.inf^M
+  SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf^M
   ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
 
   CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
@@ -108,6 +109,7 @@
 
 #### Sample Applications.
   AppPkg/Applications/Hello/Hello.inf        # No LibC includes or functions.
+  AppPkg/Applications/Pacman/Pacman.inf        # No LibC includes or functions.^M
   AppPkg/Applications/Main/Main.inf          # Simple invocation. No other LibC functions.
   AppPkg/Applications/Enquire/Enquire.inf    #
   AppPkg/Applications/ArithChk/ArithChk.inf  #
@@ -136,7 +138,7 @@
 #  AppPkg/Applications/Lua/Lua.inf
 

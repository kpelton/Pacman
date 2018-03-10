/** @file
    A simple, basic, EDK II native, "hello" application to verify that
    we can build applications without LibC.

    Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution. The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/
#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
#include  <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
/***
  Print a welcoming message.

  Establishes the main structure of the application.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
//  EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput;
    EFI_GRAPHICS_OUTPUT_PROTOCOL        *Gop;

    EFI_STATUS      status;
    UINTN           handleCount;
    EFI_HANDLE      *handleBuffer;

    status = gBS->LocateHandleBuffer(
                    ByProtocol,
                    &gEfiGraphicsOutputProtocolGuid,
                    NULL,
                    &handleCount,
                    &handleBuffer);
  if (!EFI_ERROR(status)) {
     Print(L"Found gop protocol\n");
  }


    status = gBS->HandleProtocol(
                    handleBuffer[0],    // TODO
                    &gEfiGraphicsOutputProtocolGuid,
                    (VOID **)&(Gop));

  if (!EFI_ERROR(status)) {
     Print(L"gop Ready\n");
  }

  Gop->SetMode(Gop,Gop->Mode->MaxMode);
  Print(L"H%d_V:%d\n",  Gop->Mode->Info->HorizontalResolution,
             Gop->Mode->Info->VerticalResolution);


  return(0);
}

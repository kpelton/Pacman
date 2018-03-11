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
#include <Library/BmpSupportLib.h>
#include "Pacman.h"

/***
  Print a welcoming message.

  Establishes the main structure of the application.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/

;
struct PacMan{
  EFI_GRAPHICS_OUTPUT_PROTOCOL        *Gop;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Tiles[28];
};

STATIC
VOID
InitGop (
  IN struct PacMan *Pac
);

STATIC
VOID
LoadTiles (
   IN struct PacMan *Pac
);

STATIC
VOID
LoadTiles (
  IN struct PacMan *Pac
  )
{
    UINTN                         BltSize;
    UINTN                         Height;
    UINTN                         Width;
    UINTN                         i;
     for (i=0; i<MAX_TILES; i++) {
          TranslateBmpToGopBlt (
                      Tiles[i],
                      TILE_SIZE,
                      &Pac->Tiles[i],
                      &BltSize,
                      &Height,
                      &Width
                      );
        }
}
STATIC
VOID
InitGop (
  IN struct PacMan *Pac
  )
  {
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
                    (VOID **)&(Pac->Gop));

  if (!EFI_ERROR(status)) {
     Print(L"Found gop protocol\n");
  }


}



INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
//  EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput;
  UINTN                         i;
  UINTN                         j;
  struct PacMan Pac;
  for (i=0; i<MAX_TILES; i++) {
    Pac.Tiles[i] = NULL;
  }


  InitGop(&Pac);
  LoadTiles(&Pac);

  Pac.Gop->SetMode(Pac.Gop,Pac.Gop->Mode->MaxMode);

  for (j=0; j<25; j++) {
    for (i=0; i<23; i++) {
      Pac.Gop->Blt (
               Pac.Gop,
               Pac.Tiles[i],
               EfiBltBufferToVideo,
               0,
               0,
               i*TILE_WIDTH,
               j*TILE_HEIGHT,
               TILE_WIDTH,
               TILE_HEIGHT,
               TILE_WIDTH * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
               );
    }
  }
  return(0);
}

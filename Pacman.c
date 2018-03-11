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
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Tiles[MAX_TILES];
  UINTN TileMap[MAX_Y][MAX_X];
  UINTN PacX;
  UINTN PacY;
};

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
    Pac->Tiles[i] = NULL;
  }

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
  Pac->Gop->SetMode(Pac->Gop,Pac->Gop->Mode->MaxMode);


}
STATIC
VOID
DrawBoard (
  IN struct PacMan *Pac
  ) 
{
  UINTN                         i;
  UINTN                         j;


  for (j=0; j<MAX_Y; j++) {
    for (i=0; i<MAX_X; i++) {
      Pac->Gop->Blt (
               Pac->Gop,
               Pac->Tiles[TileMap[j][i]],
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
}
STATIC
VOID
ReadKey (
  IN struct PacMan *Pac
  )
{
  EFI_INPUT_KEY Key;
  gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);


  switch(Key.ScanCode)
  {
    case SCAN_DOWN:
      TileMap[Pac->PacY][Pac->PacX] =BLANK;
      if(Pac->PacY-1 < MAX_Y && (TileMap[Pac->PacY+1][Pac->PacX] == FOOD || TileMap[Pac->PacY+1][Pac->PacX] == BLANK ))
        Pac->PacY++;
      break;
    case SCAN_UP:
      TileMap[Pac->PacY][Pac->PacX] =BLANK;
      if(Pac->PacY-1 >= 0 && (TileMap[Pac->PacY-1][Pac->PacX] == FOOD || TileMap[Pac->PacY-1][Pac->PacX] == BLANK ))
        Pac->PacY--;
      break;
    case SCAN_LEFT:
      TileMap[Pac->PacY][Pac->PacX] =BLANK;
      if(Pac->PacX>= 0 && (TileMap[Pac->PacY][Pac->PacX-1] == FOOD || TileMap[Pac->PacY][Pac->PacX-1] == BLANK ))
        Pac->PacX--;
      break;

    case SCAN_RIGHT:
      TileMap[Pac->PacY][Pac->PacX] =BLANK;
      if(Pac->PacX-1 < MAX_X && (TileMap[Pac->PacY][Pac->PacX+1] == FOOD || TileMap[Pac->PacY][Pac->PacX+1] == BLANK ))
        Pac->PacX++;
      break;
    default:
      break;
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

  struct PacMan Pac;


  InitGop(&Pac);
  LoadTiles(&Pac);
  Pac.PacX = PAC_START_X;
  Pac.PacY = PAC_START_Y;


  //Main Game loop
  for (;;) {
    ReadKey(&Pac);
    TileMap[Pac.PacY][Pac.PacX] = PAC;
    DrawBoard(&Pac);
  }
  return(0);
}

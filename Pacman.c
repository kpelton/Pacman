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
  UINTN TileMap[25][23];
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
STATIC
VOID
DrawBoard (
  IN struct PacMan *Pac
  ) 
{
  UINTN                         i;
  UINTN                         j;


  for (j=0; j<25; j++) {
    for (i=0; i<21; i++) {
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

INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
//  EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput;
  UINTN                         i;
    EFI_INPUT_KEY Key;
  struct PacMan Pac;
  UINTN PacX;
  UINTN PacY;
  for (i=0; i<MAX_TILES; i++) {
    Pac.Tiles[i] = NULL;
  }


  InitGop(&Pac);
  LoadTiles(&Pac);
  DrawBoard(&Pac);
  Pac.Gop->SetMode(Pac.Gop,Pac.Gop->Mode->MaxMode);
  PacX = PAC_START_X;
  PacY = PAC_START_Y;



  for (;;) {
    gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);


      switch(Key.ScanCode) 
      {
          case SCAN_DOWN:
              TileMap[PacY][PacX] =BLANK;
              if(PacY-1 < 25 && (TileMap[PacY+1][PacX] == FOOD || TileMap[PacY+1][PacX] == BLANK )) 
                PacY++;
          break;
          
          case SCAN_UP:
             TileMap[PacY][PacX] =BLANK;
             if(PacY-1 >= 0 && (TileMap[PacY-1][PacX] == FOOD || TileMap[PacY-1][PacX] == BLANK ))
                PacY--;
          break;
          case SCAN_LEFT:
              TileMap[PacY][PacX] =BLANK;
              if(PacX>= 0 && (TileMap[PacY][PacX-1] == FOOD || TileMap[PacY][PacX-1] == BLANK ))
              PacX--;
          break;
          
          case SCAN_RIGHT:
             TileMap[PacY][PacX] =BLANK;
             if(PacX-1 < 21 && (TileMap[PacY][PacX+1] == FOOD || TileMap[PacY][PacX+1] == BLANK )) 
              PacX++;
          break;
          
          default:  break;
    }
    TileMap[PacY][PacX] = PAC;
    DrawBoard(&Pac);
  }
  return(0);
}

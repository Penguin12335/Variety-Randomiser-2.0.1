// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "PuzzleList.h"
#include "Watchdog.h"

void PuzzleList::GenerateAllN()
{
	generator->setLoadingData(336);
	CopyTargets();
	GenerateTutorialN();
	GenerateSymmetryN();
	GenerateQuarryN();
	//GenerateBunkerN(); //Can't randomize because panels refuse to render the symbols
	GenerateSwampN();
	GenerateTreehouseN();
	GenerateTownN();
	GenerateVaultsN();
	GenerateTrianglePanelsN();
	GenerateOrchardN();
	GenerateDesertN();
	GenerateKeepN();
	GenerateJungleN();
	GenerateMountainN();
	GenerateCavesN();
	SetWindowText(_handle, L"Done!");
	(new ArrowWatchdog(0x0056E))->start(); //Easy way to close the randomizer when the game is done
	//GenerateShadowsN(); //Can't randomize
	//GenerateMonasteryN(); //Can't randomize
}

void PuzzleList::GenerateAllH()
{
	generator->setLoadingData(349);
	CopyTargets();
	GenerateTutorialH();
	GenerateSymmetryH();
	GenerateQuarryH();
	//GenerateBunkerH(); //Can't randomize because panels refuse to render the symbols
	GenerateSwampH();
	GenerateTreehouseH();
	GenerateTownH();
	GenerateVaultsH();
	GenerateTrianglePanelsH();
	GenerateOrchardH();
	GenerateDesertH();
	GenerateKeepH();
	GenerateJungleH();
	GenerateMountainH();
	GenerateCavesH();
	SetWindowText(_handle, L"Done!");
	//GenerateShadowsH(); //Can't randomize
	//GenerateMonasteryH(); //Can't randomize
}

void PuzzleList::CopyTargets()
{
	Special::copyTarget(0x00021, 0x19650);
	Special::copyTarget(0x00061, 0x09DE0);
	Special::copyTarget(0x17CFB, 0x28B39);
	Special::copyTarget(0x3C12B, 0x28B39);
	Special::copyTarget(0x17CE7, 0x17CA4);
	Special::copyTarget(0x00B8D, 0x28B39);
	Special::copyTarget(0x17FA9, 0x17CA4);
	Special::copyTarget(0x17FA0, 0x17CAB);
	Special::copyTarget(0x17D27, 0x17CAB);
	Special::copyTarget(0x17D28, 0x19650);
	Special::copyTarget(0x17D01, 0x09DE0);
	Special::copyTarget(0x17C71, 0x19650);
	Special::copyTarget(0x17CF7, 0x28B39);
	Special::copyTarget(0x17D01, 0x09DE0);
	Special::copyTarget(0x17F9B, 0x17CAB);
	Special::copyTarget(0x17C42, 0x09DE0);
	Special::copyTarget(0x00A5B, 0x17CA4);

	Special::setPower(0x17CA4, true);
	Special::setPower(0x17CAB, true);
	Special::setPower(0x28B39, true);
}

void PuzzleList::GenerateTutorialN()
{
	generator->setLoadingData(L"Tutorial", 21);
	generator->resetConfig();
	Special::drawSeedAndDifficulty(0x00064, seed, false, !seedIsRNG, false);
	Special::drawGoodLuckPanel(0x00182);
	//Mazes
	generator->setFlag(Generate::Config::FullGaps);
	generator->setGridSize(6, 6);
	generator->generateMaze(0x00293);
	generator->setGridSize(9, 9);
	generator->generateMaze(0x00295, 1, 1);
	generator->setGridSize(12, 12);
	generator->generateMaze(0x002C2);
	generator->resetConfig();
	//2 starts maze
	generator->generateMaze(0x0A3B2);
	//2 exits maze
	generator->setSymbol(Decoration::Exit, 12, 12);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generateMaze(0x0A3B5);
	generator->_panel->_endpoints.emplace_back(Endpoint(12, 0, Endpoint::Direction::RIGHT, Decoration::Exit));
	generator->write(0x0A3B5);
	generator->resetConfig();
	//Secret back area
	generator->setGridSize(4, 4);
	generator->generate(0x0A171, Decoration::Start, 1, Decoration::Dot_Intersection, 25, Decoration::Stone | Decoration::Color::Black, 3, Decoration::Stone | Decoration::Color::White, 3);
	generator->setGridSize(5, 5);
	generator->generate(0x04CA4, Decoration::Start, 1, Decoration::Dot_Intersection, 36, Decoration::Poly | Decoration::Color::Yellow, 2);
	generator->resetConfig();
	//Dots Tutorial
	generator->setGridSize(4, 4);
	generator->generate(0x0005D, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot_Intersection, 25, Decoration::Stone | Decoration::Color::Black, 4, Decoration::Stone | Decoration::Color::Magenta, 4);
	generator->generate(0x0005E, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot_Intersection, 25, Decoration::Star | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 4);
	generator->generate(0x0005F, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot_Intersection, 25, Decoration::Poly | Decoration::Color::Black, 2, Decoration::Poly | Decoration::Negative, 2);
	generator->generate(0x00060, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot_Intersection, 25, Decoration::Eraser | Decoration::Color::Black, 1, Decoration::Star| Decoration::Color::Black, 3, Decoration::Stone | Decoration::Color::Black, 3);
	generator->generate(0x00061, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot_Intersection, 25, Decoration::Triangle | Decoration::Color::Black, 6);
	//Stones Tutorial
	generator->resetConfig();
	generator->setGridSize(5, 5);
	generator->setFlagOnce(Generate::Config::WriteDotColor2);
	generator->generate(0x018AF, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot, 12);
	generator->generate(0x0001B, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Stone | Decoration::Color::Orange, 7, Decoration::Stone | Decoration::Color::White, 8);
	generator->generate(0x012C9, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Star | Decoration::Color::Orange, 6, Decoration::Star | Decoration::Color::White, 8);
	generator->generate(0x0001C, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Stone | Decoration::Color::Orange, 3, Decoration::Stone | Decoration::Color::White, 3, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::White, 3);
	generator->generate(0x0001D, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Eraser, 1, Decoration::Stone | Decoration::Color::Orange, 3, Decoration::Stone | Decoration::Color::White, 3, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::White, 3);
	generator->generate(0x0001E, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Poly, 3);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(0x0001F, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Poly | Decoration::Can_Rotate, 3);
	generator->generate(0x00020, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 1);
	generator->generate(0x00021, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Triangle, 9);
}

void PuzzleList::GenerateSymmetryN()
{
	generator->setLoadingData(L"Symmetry", 33);
	generator->resetConfig();
	generator->setFlag(Generate::Config::StartEdgeOnly);
	//Vertical Symmetry Mazes
	generator->setSymmetry(Panel::Symmetry::Vertical);
	generator->setGridSize(5, 5);
	generator->generateMaze(0x00086, 0, 1);
	generator->generateMaze(0x00087, 1, 1);
	generator->setGridSize(0, 0);
	generator->removeFlagOnce(Generate::Config::StartEdgeOnly);
	generator->generateMaze(0x00059, 1, 0);
	generator->generateMaze(0x00062);
	specialCase->generateSpecialSymMaze(generator, 0x0005C);
	//Rotational Symmetry Mazes
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setFlag(Generate::Config::WriteDotColor2);
	generator->setGridSize(5, 5);
	generator->generateMaze(0x0008D, 0, 1);
	generator->generate(0x0008D, Decoration::Dot, 7, Decoration::Gap, 11);
	generator->generateMaze(0x00081, 1, 1);
	generator->generate(0x00081, Decoration::Dot, 9, Decoration::Gap, 11);
	generator->removeFlagOnce(Generate::Config::StartEdgeOnly);
	generator->generateMaze(0x00083, 1, 1);
	generator->generate(0x00083, Decoration::Dot, 11, Decoration::Gap, 11);
	generator->setGridSize(6, 6);
	generator->generateMaze(0x00084);
	generator->generate(0x00084, Decoration::Dot, 11, Decoration::Gap, 15);
	generator->generateMaze(0x00082);
	generator->generate(0x00082, Decoration::Dot, 13, Decoration::Gap, 15);
	generator->generateMaze(0x0343A);
	generator->generate(0x0343A, Decoration::Dot, 15, Decoration::Gap, 15);
	//Symmetry Island Door
	generator->resetConfig();
	generator->setGridSize(5, 5);
	generator->setFlagOnce(Generate::Config::FullGaps);
	generator->generate(0x000B0, Decoration::Dot, 12, Decoration::Gap, 9);
	//Black Dots
	generator->setFlag(Generate::Config::StartEdgeOnly);
	generator->setGridSize(5, 5);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(0x00022, Decoration::Dot, 36, Decoration::Triangle, 5, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x00023, Decoration::Dot, 36, Decoration::Triangle, 6, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x00024, Decoration::Dot, 36, Decoration::Triangle, 7, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x00025, Decoration::Dot, 36, Decoration::Triangle, 8, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x00026, Decoration::Dot, 36, Decoration::Triangle, 9, Decoration::Start, 1, Decoration::Exit, 1);
	//Colored Dots
	generator->generate(0x0007C, Decoration::Dot, 3, Decoration::Dot | Decoration::Color::Blue, 3, Decoration::Dot | Decoration::Color::Yellow, 3, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x0007E, Decoration::Dot, 4, Decoration::Dot | Decoration::Color::Blue, 3, Decoration::Dot | Decoration::Color::Yellow, 4, Decoration::Start, 1, Decoration::Exit, 2);
	generator->generate(0x00075, Decoration::Dot | Decoration::Color::Blue, 3, Decoration::Dot | Decoration::Color::Yellow, 4, Decoration::Dot, 5, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x00073, Decoration::Eraser, 1, Decoration::Dot | Decoration::Color::Blue, 3, Decoration::Dot | Decoration::Color::Yellow, 3, Decoration::Dot, 3, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x00077, Decoration::Eraser, 1, Decoration::Dot | Decoration::Color::Blue, 4, Decoration::Dot | Decoration::Color::Yellow, 4, Decoration::Dot, 4, Decoration::Start, 1, Decoration::Exit, 2);
	generator->generate(0x00079, Decoration::Eraser, 1, Decoration::Dot | Decoration::Color::Blue, 5, Decoration::Dot | Decoration::Color::Yellow, 5, Decoration::Dot, 5, Decoration::Start, 1, Decoration::Exit, 1);
	//Fading Lines
	generator->setGridSize(5, 5);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(0x00065, Decoration::Dot | Decoration::Color::Blue, 4, Decoration::Dot | Decoration::Color::Yellow, 4, Decoration::Dot, 4, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x0006D, Decoration::Stone | Decoration::Color::Cyan, 6, Decoration::Stone | Decoration::Color::Yellow, 6, Decoration::Start, 1, Decoration::Exit, 2);
	generator->generate(0x00072, Decoration::Star | Decoration::Color::Cyan, 6, Decoration::Star | Decoration::Color::Yellow, 8, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x0006F, Decoration::Start, 1, Decoration::Stone | Decoration::Color::Cyan, 6, Decoration::Star | Decoration::Color::Cyan, 4, Decoration::Exit, 1);
	generator->generate(0x00070, Decoration::Start, 1, Decoration::Stone | Decoration::Color::Yellow, 6, Decoration::Star | Decoration::Color::Yellow, 4, Decoration::Exit, 1);
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x00071, Decoration::Start, 1, Decoration::Poly | Decoration::Color::Yellow, 3);
	generator->generate(0x00076, Decoration::Start, 1, Decoration::Eraser, 1, Decoration::Dot, 12);
	//Dot Reflection Dual Panels (before laser)
	generator->resetConfig();
	std::set<Panel::Symmetry> normalSym = { Panel::Symmetry::Horizontal, Panel::Symmetry::Rotational };
	std::set<Panel::Symmetry> weirdSym = { Panel::Symmetry::RotateLeft, Panel::Symmetry::RotateRight, Panel::Symmetry::FlipXY, Panel::Symmetry::FlipNegXY };
	specialCase->generateReflectionDotPuzzle(generator, 0x00A52, 0x00A61, { {Decoration::Dot, 10 },
		{Decoration::Exit, 1}, { Decoration::Gap, 5 } }, Panel::Symmetry::Vertical, false);
	generator->setSymbol(Decoration::Start, 0, 8);
	specialCase->generateReflectionDotPuzzle(generator, 0x00A57, 0x00A64, { { Decoration::Dot, 11},
		{ Decoration::Exit, 1}, {Decoration::Gap, 1} }, pop_random(normalSym), false);
	generator->setSymbol(Decoration::Start, 0, 8); generator->setSymbol(Decoration::Start, 8, 8); generator->setSymbol(Decoration::Start, 8, 0); generator->setSymbol(Decoration::Start, 0, 0);
	generator->setSymbol(Decoration::Exit, 8, 2);
	specialCase->generateReflectionDotPuzzle(generator, 0x00A5B, 0x00A68, { { Decoration::Dot, 12} }, pop_random(weirdSym), false);
}

void PuzzleList::GenerateQuarryN()
{
	generator->setLoadingData(L"Quarry", 39);
	generator->resetConfig();
	//Entry Gates
	generator->setFlag(Generate::Config::EnableFlash);
	generator->setGridSize(5, 5);
	generator->generate(0x09E57, Decoration::Stone | Decoration::Color::Black, 5, Decoration::Stone | Decoration::Color::White, 7, Decoration::Dot, 7);
	generator->setGridSize(4, 4);
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->generate(0x17C09, Decoration::Eraser, 1, Decoration::Poly, 3, Decoration::Gap, 5);
	//Mill Entry Door
	generator->resetConfig();
	generator->setFlag(Generate::Config::EnableFlash);
	generator->setGridSize(5, 5);
	generator->generate(0x01E5A, Decoration::Eraser | Decoration::Color::White, 1, Decoration::Star | Decoration::Color::White, 5, Decoration::Star | Decoration::Color::Black, 6, Decoration::Star | Decoration::Color::Orange, 6);
	generator->setFlagOnce(Generate::Config::PreserveStructure);
	generator->generate(0x01E59, Decoration::Triangle | Decoration::Black, 7);
	//Dots
	generator->setGridSize(4, 4);
	generator->generate(0x00E0C, Decoration::Dot, 7, Decoration::Eraser | Decoration::Color::Green, 2);
	generator->generate(0x01489, Decoration::Dot, 9, Decoration::Eraser | Decoration::Color::Green, 2);
	generator->generate(0x0148A, Decoration::Dot, 11, Decoration::Eraser | Decoration::Color::Green, 2);
	generator->setGridSize(5, 5);
	generator->generate(0x014D9, Decoration::Dot, 13, Decoration::Eraser | Decoration::Color::Green, 2);
	generator->generate(0x014E7, Decoration::Dot, 15, Decoration::Eraser | Decoration::Color::Green, 2);
	generator->generate(0x014E8, Decoration::Dot, 17, Decoration::Eraser | Decoration::Color::Green, 2);
	//Stones
	generator->setFlag(Generate::Config::AlternateColors);
	generator->setGridSize(4, 4); //green -> white; magenta -> blue; Black -> red; white -> green
	generator->generate(0x00557, Decoration::Stone | Decoration::Color::White, 3, Decoration::Stone | Decoration::Color::Black, 3, Decoration::Stone | Decoration::Color::Magenta, 3, Decoration::Eraser | Decoration::Color::Black, 1, Decoration::Eraser | Decoration::Color::White, 1);
	generator->generate(0x005F1, Decoration::Stone | Decoration::Color::White, 4, Decoration::Stone | Decoration::Color::Black, 4, Decoration::Stone | Decoration::Color::Magenta, 4, Decoration::Eraser | Decoration::Color::Magenta, 1, Decoration::Eraser | Decoration::Color::White, 1);
	generator->setGridSize(5, 5);
	generator->generate(0x00620, Decoration::Stone | Decoration::Color::White, 5, Decoration::Stone | Decoration::Color::Black, 6, Decoration::Stone | Decoration::Color::Magenta, 5, Decoration::Eraser | Decoration::Color::Magenta, 1, Decoration::Eraser | Decoration::Color::Black, 1);
	generator->generate(0x009F5, Decoration::Stone | Decoration::Color::White, 6, Decoration::Stone | Decoration::Color::Black, 5, Decoration::Stone | Decoration::Color::Magenta, 7, Decoration::Eraser | Decoration::Color::Black, 1, Decoration::Eraser | Decoration::Color::White, 1);
	generator->setGridSize(4, 4);
	generator->generate(0x0146C, Decoration::Star | Decoration::Color::White, 3, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::Magenta, 3, Decoration::Eraser | Decoration::Color::Magenta, 1, Decoration::Eraser | Decoration::Color::White, 1);
	generator->generate(0x3C12D, Decoration::Star | Decoration::Color::White, 4, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Eraser | Decoration::Color::Magenta, 1, Decoration::Eraser | Decoration::Color::Black, 1);
	generator->setGridSize(5, 5);
	generator->generate(0x03686, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::White, 5, Decoration::Star | Decoration::Color::Black, 5, Decoration::Eraser | Decoration::Color::Black, 1, Decoration::Eraser | Decoration::Color::White, 1);
	generator->generate(0x014E9, Decoration::Star | Decoration::Color::White, 5, Decoration::Star | Decoration::Color::Black, 6, Decoration::Star | Decoration::Color::Magenta, 5, Decoration::Eraser | Decoration::Color::Magenta, 1, Decoration::Eraser | Decoration::Color::White, 1);
	//Dots + Stones
	generator->resetConfig();
	generator->setGridSize(5, 5);
	generator->setSymbol(Decoration::Start, 0, 10);
	generator->setFlagOnce(Generate::Config::AlternateColors);
	generator->generate(0x0367C, Decoration::Eraser | Decoration::Color::Magenta, 1, Decoration::Eraser | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::White, 3, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::Magenta, 1,
		Decoration::Star | Decoration::Color::White, 3, Decoration::Star | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::Magenta, 2,
		Decoration::Dot, 5);
	generator->setGridSize(7, 7);
	generator->generate(0x3C125, Decoration::Eraser | Decoration::Color::White, 1, Decoration::Triangle | Decoration::Color::White, 13, Decoration::Dot_Intersection, 64, Decoration::Start, 3, Decoration::Exit, 3);
	//Boathouse Ramp Activation
	generator->setGridSize(5, 5);
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(0x034D4, Decoration::Star | Decoration::White, 8, Decoration::Star | Decoration::Black, 6, Decoration::Star | Decoration::Magenta, 4);
	generator->setGridSize(4, 4);
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->generate(0x021D5, Decoration::Poly, 1, Decoration::Poly | Decoration::Can_Rotate, 1, Decoration::Triangle1, 1, Decoration::Triangle2, 1, Decoration::Triangle3, 1);
	//Eraser + Shapes
	generator->setFlag(Generate::Config::ResetColors);
	generator->setGridSize(3, 3);
	generator->generate(0x021B3, Decoration::Poly, 4, Decoration::Eraser | Decoration::Color::White, 2);
	generator->setGridSize(4, 4);
	generator->generate(0x021B4, Decoration::Poly, 4, Decoration::Eraser | Decoration::Color::White, 2);
	generator->generate(0x021B0, Decoration::Poly, 4, Decoration::Eraser | Decoration::Color::White, 2);
	generator->generate(0x021AF, Decoration::Poly, 5, Decoration::Eraser | Decoration::Color::White, 2);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(0x021AE, Decoration::Poly, 4, Decoration::Eraser | Decoration::Color::White, 2);
	generator->removeFlag(Generate::Config::ResetColors);
	//Eraser + Stars
	generator->setGridSize(3, 3);
	generator->generate(0x021B5, Decoration::Eraser | Decoration::Color::White, 2, Decoration::Poly | Decoration::Color::Green, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::Magenta, 2);
	generator->setGridSize(4, 4);
	generator->generate(0x021B6, Decoration::Eraser | Decoration::Color::White, 2, Decoration::Poly | Decoration::Color::Green, 3, Decoration::Poly | Decoration::Negative | Decoration::Color::Magenta, 2);
	generator->generate(0x021B7, Decoration::Eraser | Decoration::Color::White, 2, Decoration::Poly | Decoration::Color::Green, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::Magenta, 3);
	generator->generate(0x021BB, Decoration::Eraser | Decoration::Color::White, 2, Decoration::Poly | Decoration::Color::Green, 3, Decoration::Poly | Decoration::Negative | Decoration::Color::Magenta, 3);
	generator->generate(0x09DB5, Decoration::Eraser | Decoration::Color::White, 2, Decoration::Poly | Decoration::Color::Green, 3, Decoration::Poly | Decoration::Negative | Decoration::Color::Magenta, 4);
	//Eraser + Stars + Shapes
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->generate(0x09DB1, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Orange, 1, Decoration::Stone | Decoration::Color::Magenta, 2, Decoration::Eraser | Decoration::Magenta, 1, Decoration::Eraser | Decoration::Orange, 1);
	generator->generate(0x3C124, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::Magenta, 1, Decoration::Stone | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::Magenta, 2, Decoration::Eraser | Decoration::Magenta, 1, Decoration::Eraser | Decoration::Orange, 1);
	generator->generate(0x09DB3, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Triangle | Decoration::Color::Orange, 1, Decoration::Stone | Decoration::Color::Magenta, 2, Decoration::Eraser | Decoration::Magenta, 1, Decoration::Eraser | Decoration::Orange, 1);
	generator->generate(0x09DB4, Decoration::Star | Decoration::Color::Orange, 1, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Triangle | Decoration::Color::Orange, 3, Decoration::Stone | Decoration::Color::Magenta, 2, Decoration::Eraser | Decoration::Magenta, 1, Decoration::Eraser | Decoration::Orange, 1);
	generator->setGridSize(5, 5);
	generator->generate(0x0A3CB, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Green, 2, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Orange, 1, Decoration::Stone | Decoration::Color::Green, 2, Decoration::Stone | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::Magenta, 3, Decoration::Eraser | Decoration::Orange, 1, Decoration::Eraser | Decoration::Magenta, 1);
	generator->generate(0x0A3CC, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Green, 2, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Magenta, 1, Decoration::Star | Decoration::Color::Green, 2, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Eraser | Decoration::Green, 1, Decoration::Eraser | Decoration::Magenta, 1);
	generator->generate(0x0A3D0, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Magenta, 2, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Orange, 1, Decoration::Triangle | Decoration::Color::Green, 2, Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Triangle | Decoration::Color::Magenta, 2, Decoration::Eraser | Decoration::Orange, 1, Decoration::Eraser | Decoration::Green, 1);
}

void PuzzleList::GenerateBunkerN()
{
	//I would randomize this, if I could get the panels to actually render the symbols.
	//Unfortunately, the path is rendered to a 3D model that doesn't have any geometry between the grid lines.
	//Somehow, I would either have to change the model, or make the puzzle render to the background texture instead.
}

void PuzzleList::GenerateSwampN()
{
	generator->setLoadingData(L"Swamp", 49);
	generator->resetConfig();
	//First row
	generator->setGridSize(3, 3);
	generator->pathWidth = 0.7f;
	generator->generate(0x0056E, Decoration::Poly, 2, Decoration::Stone | Decoration::Color::White, 1, Decoration::Stone | Decoration::Color::Black, 1);
	generator->setFlag(Generate::Config::RequireCombineShapes);
	generator->generate(0x00469, Decoration::Poly, 2);
	generator->generate(0x00472, Decoration::Poly, 2);
	generator->setGridSize(4, 4);
	generator->generate(0x00262, Decoration::Poly, 3);
	generator->generate(0x00474, Decoration::Poly, 3);
	generator->setGridSize(5, 5);
	generator->generate(0x00553, Decoration::Poly, 4);
	generator->generate(0x0056F, Decoration::Poly, 4);
	//Second Row
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(0x00390, Decoration::Poly, 2, Decoration::Stone | Decoration::Color::White, 3, Decoration::Stone | Decoration::Color::Black, 2);
	generator->generate(0x010CA, Decoration::Poly, 2, Decoration::Stone | Decoration::Color::White, 3, Decoration::Stone | Decoration::Color::Black, 4);
	generator->generate(0x00983, Decoration::Poly, 1, Decoration::Poly | Decoration::Can_Rotate, 1, Decoration::Stone | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::Black, 3);
	generator->generate(0x00984, Decoration::Poly, 1, Decoration::Poly | Decoration::Can_Rotate, 1, Decoration::Stone | Decoration::Color::White, 3, Decoration::Stone | Decoration::Color::Black, 2);
	generator->generate(0x00986, Decoration::Poly, 2, Decoration::Triangle, 2);
	generator->generate(0x00985, Decoration::Poly, 2, Decoration::Triangle, 3);
	generator->generate(0x00987, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Triangle, 2);
	generator->pathWidth = 0.6f;
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x181A9, Decoration::Poly, 3, Decoration::Triangle, 3);
	//Turn off bridge control until all previous puzzles are solved
	specialCase->setTargetAndDeactivate(0x181A9, 0x00609);
	//Red Panels
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->setSymbol(Decoration::Start, 4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(0x00982, Decoration::Poly, 2, Decoration::Gap, 5);
	generator->setSymbol(Decoration::Start, 4, 4);
	generator->generate(0x0097F, Decoration::Poly, 2, Decoration::Gap, 5);
	generator->setSymbol(Decoration::Start, 4, 4);
	generator->generate(0x0098F, Decoration::Poly, 3, Decoration::Gap, 5);
	generator->setSymbol(Decoration::Start, 4, 4);
	generator->generate(0x00990, Decoration::Poly, 4, Decoration::Gap, 5);
	generator->setGridSize(5, 5);
	generator->setFlagOnce(Generate::Config::DisableReset);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(0x17C0D, Decoration::Poly, 3);
	generator->place_gaps(15);
	generator->write(0x17C0E);
	//Disconnected Shapes
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->setFlag(Generate::Config::DisconnectShapes);
	generator->pathWidth = 0.7f;
	generator->generate(0x00999, Decoration::Poly, 2);
	generator->pathWidth = 0.65f;
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x0099D, Decoration::Poly, 2);
	generator->generate(0x009A0, Decoration::Poly, 3);
	generator->setGridSize(5, 5);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x009A1, Decoration::Poly, 3);
	//Rotating Shapes
	generator->resetConfig();
	generator->setGridSize(3, 3);
	generator->generate(0x00007, Decoration::Poly | Decoration::Can_Rotate, 1, Decoration::Dot, 3);
	generator->generate(0x00008, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Dot, 3);
	generator->setGridSize(4, 4);
	generator->generate(0x00009, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Dot, 5);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x0000A, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Dot, 7);
	//5x5 Rotating Shapes
	generator->setGridSize(5, 5);
	generator->generate(0x003B2, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Dot, 5);
	generator->generate(0x00A1E, Decoration::Poly | Decoration::Can_Rotate, 4, Decoration::Dot, 5);
	generator->generate(0x00C2E, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Dot, 7);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x00E3A, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Dot, 7);
	//Optional Tetris
	generator->resetConfig();
	generator->setGridSize(6, 6);
	generator->generate(0x009A6, Decoration::Poly, 5, Decoration::Dot, 7);
	//Negative Shapes 1
	generator->resetConfig();
	generator->setFlag(Generate::Config::RequireCombineShapes);
	generator->setFlag(Generate::Config::DisableCancelShapes);
	generator->setGridSize(3, 3);
	generator->pathWidth = 0.65f;
	generator->generate(0x009AB, Decoration::Poly, 2, Decoration::Poly | Decoration::Negative, 1);
	generator->generate(0x009AD, Decoration::Poly, 2, Decoration::Poly | Decoration::Negative, 2);
	generator->removeFlag(Generate::Config::RequireCombineShapes);
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(0x009AE, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 2);
	generator->generate(0x009AF, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 3);
	generator->removeFlag(Generate::Config::BigShapes);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(0x00006, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 2);
	//Negative Shapes 2
	generator->setGridSize(4, 4);
	generator->generate(0x00002, Decoration::Poly, 2, Decoration::Poly | Decoration::Negative, 2, Decoration::Stone | Decoration::Color::White, 3, Decoration::Stone | Decoration::Color::Black, 2);
	generator->generate(0x00004, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 3, Decoration::Stone | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::Black, 3);
	generator->generate(0x00005, Decoration::Poly, 2, Decoration::Poly | Decoration::Negative, 2, Decoration::Star | Decoration::Color::White, 4, Decoration::Star | Decoration::Color::Black, 4);
	generator->generate(0x013E6, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 3, Decoration::Star | Decoration::Color::White, 4, Decoration::Star | Decoration::Color::Black, 4);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(0x00596, Decoration::Poly, 2, Decoration::Poly | Decoration::Negative, 2, Decoration::Dot, 5);
	//Negative Shapes 3
	generator->resetConfig();
	generator->setFlag(Generate::Config::RequireCancelShapes);
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.7f;
	generator->generate(0x00001, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot, 25, Decoration::Poly, 2, Decoration::Poly | Decoration::Negative, 2);
	generator->generate(0x014D2, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot, 25, Decoration::Poly, 2, Decoration::Poly | Decoration::Negative, 3);
	generator->setFlag(Generate::Config::RequireCancelShapes);
	generator->setGridSize(5, 5);	
	generator->pathWidth = 0.7f;
	generator->generate(0x014D4, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot, 36, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 2);
	generator->generate(0x014D1, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot, 36, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 3);
	//Exit Shortcut
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->generate(0x17C05, Decoration::Poly | Decoration::Color::Orange, 3, Decoration::Stone | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::Orange, 3);
	generator->generate(0x17C02, Decoration::Poly | Decoration::Color::Blue, 3, Decoration::Stone | Decoration::Color::Blue, 3, Decoration::Star | Decoration::Color::Blue, 3);
}

void PuzzleList::GenerateTreehouseN()
{
	generator->setLoadingData(L"Treehouse", 57);
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteColors); //Have to do this to get a proper looking orange and green
	generator->setGridSize(3, 3);
	generator->generate(0x02886, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Triangle | Decoration::Color::Orange, 1, Decoration::Triangle | Decoration::Color::Magenta, 1);
	generator->setFlag(Generate::Config::TreehouseLayout);
	//Yellow Bridge
	generator->generate(0x17D72, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::Magenta, 2);
	generator->generate(0x17D8F, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 4);
	generator->setGridSize(4, 4);
	generator->generate(0x17D74, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::Magenta, 6);
	generator->generate(0x17DAC, Decoration::Star | Decoration::Color::Orange, 6, Decoration::Star | Decoration::Color::Magenta, 6);
	generator->generate(0x17D9E, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::Green, 4);
	generator->generate(0x17DB9, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Green, 6);
	generator->setGridSize(5, 5);
	generator->generate(0x17D9C, Decoration::Star | Decoration::Color::Orange, 6, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Green, 6);
	generator->generate(0x17DC2, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::Magenta, 6, Decoration::Star | Decoration::Color::Green, 2, Decoration::Star | Decoration::Color::White, 4);
	generator->generate(0x17DC4, Decoration::Star | Decoration::Color::Orange, 6, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Green, 4, Decoration::Star | Decoration::Color::White, 6);
	generator->removeFlagOnce(Generate::Config::TreehouseLayout);
	generator->setGridSize(5, 5);
	generator->generate(0x0A182, Decoration::Star | Decoration::Color::Orange, 6, Decoration::Star | Decoration::Color::Magenta, 6, Decoration::Star | Decoration::Color::Green, 6, Decoration::Star | Decoration::Color::White, 6);
	//Pink Bridge 1
	generator->setFlag(Generate::Config::FullGaps);
	generator->setGridSize(3, 3);
	generator->generate(0x17DC8, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Dot_Intersection, 2, Decoration::Triangle, 2, Decoration::Gap, 3);
	generator->setGridSize(4, 4);
	generator->generate(0x17DC7, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Dot_Intersection, 3, Decoration::Triangle, 3, Decoration::Gap, 5);
	generator->generate(0x17CE4, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Dot_Intersection, 5, Decoration::Triangle, 3, Decoration::Gap, 5);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.8f;
	generator->generate(0x17D2D, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Orange, 6, Decoration::Dot_Intersection, 7, Decoration::Triangle, 4, Decoration::Gap, 7);
	generator->generate(0x17D6C, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Dot_Intersection, 6, Decoration::Triangle, 5, Decoration::Gap, 7);
	generator->removeFlag(Generate::Config::FullGaps);
	//Pink Bridge 2
	generator->setGridSize(3, 3);
	generator->generate(0x17D9B, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::White, 1, Decoration::Star | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::Black, 1);
	generator->generate(0x17D99, Decoration::Star | Decoration::Color::Magenta, 1, Decoration::Stone | Decoration::Color::Magenta, 1, Decoration::Star | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::Black, 1);
	generator->setGridSize(4, 4);
	generator->generate(0x17DAA, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::White, 4, Decoration::Star | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::Black, 2);
	generator->generate(0x17D97, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Stone | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 4, Decoration::Stone | Decoration::Color::Black, 2);
	generator->removeFlag(Generate::Config::WriteColors);
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->setGridSize(5, 4);
	generator->generate(0x17BDF, Decoration::Star | Decoration::Color::Orange, 1, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::Green, 2, Decoration::Stone | Decoration::Color::Magenta, 3, Decoration::Stone | Decoration::Color::Orange, 2);
	generator->generate(0x17D91, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::Green, 2, Decoration::Stone | Decoration::Color::Orange, 3, Decoration::Stone | Decoration::Color::Green, 3);
	generator->generate(0x17DC6, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::Magenta, 3, Decoration::Star | Decoration::Color::Green, 2, Decoration::Stone | Decoration::Color::Magenta, 3, Decoration::Stone | Decoration::Color::Orange, 3);
	generator->removeFlag(Generate::Config::TreehouseColors);
	generator->setFlag(Generate::Config::WriteColors);
	//Orange Bridge 1		
	generator->setGridSize(3, 3);
	generator->generate(0x17DB3, Decoration::Star | Decoration::Color::Black, 1, Decoration::Triangle | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::White, 1, Decoration::Triangle | Decoration::Color::White, 1);
	generator->generate(0x17DB5, Decoration::Star | Decoration::Color::Black, 2, Decoration::Triangle | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::White, 1, Decoration::Triangle | Decoration::Color::White, 1);
	generator->generate(0x17DB6, Decoration::Star | Decoration::Color::Black, 1, Decoration::Triangle | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 1, Decoration::Triangle | Decoration::Color::White, 2);
	generator->generate(0x17DC0, Decoration::Star | Decoration::Color::Black, 2, Decoration::Triangle | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::White, 2, Decoration::Triangle | Decoration::Color::White, 2);
	generator->setGridSize(4, 4);
	//Black -> Green, White -> Pink, Purple -> White
	generator->generate(0x17DD7, Decoration::Star | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Poly | Decoration::Color::Black, 2);
	generator->generate(0x17DD9, Decoration::Star | Decoration::Color::White, 3, Decoration::Star | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::Black, 1, Decoration::Poly | Decoration::Color::White, 2);
	generator->generate(0x17DB8, Decoration::Star | Decoration::Color::White, 3, Decoration::Star | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::Black, 3, Decoration::Poly | Decoration::Color::White, 1, Decoration::Poly | Decoration::Color::Black, 1);
	generator->generate(0x17DDC, Decoration::Star | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 3, Decoration::Stone | Decoration::Color::White, 3, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Poly | Decoration::Color::White, 1, Decoration::Poly | Decoration::Color::Black, 1);
	generator->generate(0x17DD1, Decoration::Star | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 1);
	//last half orange 1
	generator->setGridSize(5, 5);
	generator->generate(0x17DDE, Decoration::Star | Decoration::Color::Black, 8, Decoration::Poly | Decoration::Color::Black, 2);
	generator->generate(0x17DE3, Decoration::Star | Decoration::Color::White, 8,  Decoration::Poly | Decoration::Color::White, 2);
	generator->generate(0x17DEC, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2, Decoration::Poly | Decoration::Color::White, 1, Decoration::Poly | Decoration::Color::Black, 1);
	generator->generate(0x17DAE, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 2, Decoration::Triangle | Decoration::Color::Black, 2, Decoration::Triangle | Decoration::Color::White, 2, Decoration::Poly | Decoration::Color::White, 1, Decoration::Poly | Decoration::Color::Black, 1);
	generator->generate(0x17DB0, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 3, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 1, Decoration::Poly | Decoration::Color::White, 2, Decoration::Poly | Decoration::Color::Black, 1);
	generator->generate(0x17DDB, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::White, 2, Decoration::Triangle | Decoration::Color::Black, 1, Decoration::Triangle | Decoration::Color::White, 2, Decoration::Poly | Decoration::Color::White, 1, Decoration::Poly | Decoration::Color::Black, 2);
	//Orange Bridge 2
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->setGridSize(3, 3);
	generator->generate(0x17D88, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Orange, 1, Decoration::Stone | Decoration::Color::Magenta, 1);
	generator->generate(0x17DB4, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::Magenta, 2);
	generator->generate(0x17D8C, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::Magenta, 2);
	generator->setGridSize(4, 4);
	generator->generate(0x17CE3, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Orange, 1, Decoration::Stone | Decoration::Color::Magenta, 1);
	generator->generate(0x17DCD, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 3, Decoration::Stone | Decoration::Color::Orange, 3, Decoration::Stone | Decoration::Color::Magenta, 2);
	generator->generate(0x17DB2, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::Magenta, 3);
	generator->generate(0x17DCC, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::Magenta, 3);
	generator->generate(0x17DCA, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::Magenta, 3, Decoration::Star | Decoration::Color::Green, 2, Decoration::Stone | Decoration::Color::Orange, 1, Decoration::Stone | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Green, 2);
	generator->generate(0x17D8E, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::Green, 3, Decoration::Stone | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Green, 1);
	generator->setFlagOnce(Generate::Config::FixBackground);
	generator->setGridSize(4, 4);
	generator->generate(0x17DB7, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Green, 2);
	generator->generate(0x17DB1, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Green, 2, Decoration::Stone | Decoration::Color::Orange, 3, Decoration::Stone | Decoration::Color::Magenta, 3, Decoration::Stone | Decoration::Color::Green, 2);
	generator->generate(0x17DA2, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 3, Decoration::Star | Decoration::Color::Green, 2, Decoration::Stone | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::Magenta, 3, Decoration::Stone | Decoration::Color::Green, 4);
	//Green Bridge
	generator->removeFlag(Generate::Config::TreehouseColors);
	generator->setFlag(Generate::Config::AlternateColors);
	generator->setFlag(Generate::Config::DisableCancelShapes);
	generator->setGridSize(4, 4);
	generator->generate(0x17E3C, Decoration::Poly | Decoration::Color::White, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 4);
	generator->generate(0x17E4D, Decoration::Poly | Decoration::Color::White, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::White, 2);
	generator->generate(0x17E4F, Decoration::Poly | Decoration::Color::White, 3, Decoration::Poly | Decoration::Negative | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 4);
	generator->setObstructions({ { { 1, 2 },{ 1, 4 },{ 9, 2 },{ 9, 4 },{ 2, 1 },{ 4, 1 },{ 6, 1 },{ 8, 1 } },
		{ { 1, 2 },{ 1, 4 },{ 1, 6 },{ 0, 7 },{ 9, 2 },{ 9, 4 },{ 9, 6 },{ 10, 7 },{ 4, 1 },{ 6, 1 },{ 8, 1 } },
		{ { 1, 2 },{ 1, 4 },{ 1, 6 },{ 0, 7 },{ 9, 2 },{ 9, 4 },{ 9, 6 },{ 10, 7 },{ 2, 1 },{ 4, 1 },{ 6, 1 } } });
	generator->setGridSize(5, 5);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x17E52, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::White, 2, Decoration::Poly | Decoration::Negative| Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::White, 4);
	generator->generate(0x17E5B, Decoration::Poly | Decoration::Color::White, 1, Decoration::Poly | Decoration::Color::Black, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::White, 2);
	generator->generate(0x17E5F, Decoration::Poly | Decoration::Color::White, 2, Decoration::Poly | Decoration::Color::Black, 1, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 4);
	generator->generate(0x17E61, Decoration::Poly | Decoration::Color::White, 1, Decoration::Poly | Decoration::Color::Black, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 3, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::White, 4);
}

void PuzzleList::GenerateTownN()
{
	generator->setLoadingData(L"Town", 20);
	generator->resetConfig();
	//Full Dots + Shapes
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(0x2899C, Decoration::Dot_Intersection, 25, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Stone | Decoration::Color::Yellow, 2, Decoration::Stone | Decoration::Color::Cyan, 2, Decoration::Stone | Decoration::Color::Magenta, 2);
	generator->generate(0x28A33, Decoration::Dot_Intersection, 25, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Star | Decoration::Color::Yellow, 2, Decoration::Star | Decoration::Color::Cyan, 4, Decoration::Star | Decoration::Color::Magenta, 2);
	generator->generate(0x28ABF, Decoration::Dot_Intersection, 25, Decoration::Poly | Decoration::Negative, 5, Decoration::Poly, 2);
	generator->generate(0x28AC0, Decoration::Dot_Intersection, 25, Decoration::Poly | Decoration::Can_Rotate, 3);
	generator->generate(0x28AC1, Decoration::Dot_Intersection, 25, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Triangle, 3);
	generator->generate(0x28AD9, Decoration::Dot_Intersection, 25, Decoration::Eraser | Decoration::Color::White, 1, Decoration::Poly | Decoration::Can_Rotate, 3);
	//Blue Symmetry
	generator->setFlag(Generate::Config::WriteDotColor);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(0x28AC7, Decoration::Dot, 11);
	generator->generate(0x28AC8, Decoration::Stone | Decoration::Color::Black, 7, Decoration::Stone | Decoration::Color::White, 7);
	generator->generate(0x28ACA, Decoration::Star | Decoration::Color::Black, 6, Decoration::Star | Decoration::Color::White, 8);
	generator->generate(0x28ACB, Decoration::Poly | Decoration::Color::Green, 3);
	generator->generate(0x28ACC, Decoration::Triangle | Decoration::Color::Yellow, 8);
	generator->setSymmetry(Panel::Symmetry::None);
	//Glass Door
	generator->generate(0x28998, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Star | Decoration::Color::Yellow, 6, Decoration::Star | Decoration::Color::White, 4);
	//Church Star Door
	generator->removeFlag(Generate::Config::EnableFlash);
	specialCase->generateColorFilterPuzzle(0x28A0D, { 4, 4 }, { std::make_pair<int, int>(Decoration::Star | 1, 6),
		std::make_pair<int,int>(Decoration::Star | 2, 6), std::make_pair<int,int>(Decoration::Star | 3, 4) }, { 1, 1, 0, 0 });
	specialCase->setPower(0x28A69, false); (new TownDoorWatchdog())->start();
	//Soundproof Room
	std::vector<int> allPitches = { DOT_SMALL, DOT_SMALL, DOT_MEDIUM, DOT_MEDIUM, DOT_LARGE, DOT_LARGE };
	std::vector<int> pitches;
	for (int i = 0; i < 4; i++) pitches.push_back(pop_random(allPitches));
	specialCase->generateSoundDotPuzzle(0x034E3, { 4, 4 }, pitches, false);
	generator->resetConfig();
	//3-color Room
	generator->setGridSize(6, 6);
	generator->generate(0x03C0C, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::Red, 4, Decoration::Star | Decoration::Color::Green, 6, Decoration::Star | Decoration::Color::Blue, 2);
	generator->generate(0x03C08, Decoration::Star | Decoration::Color::White, 4, Decoration::Star | Decoration::Color::Magenta, 6, Decoration::Star | Decoration::Color::Cyan, 2, Decoration::Star | Decoration::Color::Yellow, 4);
	//Orange Crate
	generator->setFlag(Generate::Config::EnableFlash);
	generator->setGridSize(6, 6);
	generator->generate(0x0A0C8, Decoration::Triangle | Decoration::Color::Orange, 15);
	//Windmill Puzzles
	//The Witness has a weird issue with these particular puzzles where the edge at (3, 4) gets bypassed by the region calculator if you don't draw over it.
	//Because of this, polyominoes can't be used with these puzzles unless Config::PreserveStructure flag is turned off.
	generator->setGridSize(5, 5);
	generator->generate(0x17F89, Decoration::Stone | Decoration::Color::Black, 5, Decoration::Stone | Decoration::Color::White, 5, Decoration::Triangle | Decoration::Color::Orange, 5);
	generator->setFlag(Generate::Config::FixBackground);
	generator->generate(0x0A168, Decoration::Stone | Decoration::Color::Black, 6, Decoration::Star | Decoration::Color::Black, 6, Decoration::Poly | Decoration::Color::Black, 3);
	generator->generate(0x33AB2, Decoration::Stone | Decoration::Color::White, 6, Decoration::Star | Decoration::Color::White, 6, Decoration::Poly | Decoration::Color::White, 3);
}

void PuzzleList::GenerateVaultsN()
{
	generator->setLoadingData(L"Vaults", 5);
	generator->resetConfig();
	//Tutorial Vault
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setGridSize(7, 7);
	generator->generate(0x033D4, Decoration::Stone | Decoration::Color::White, 12, Decoration::Stone | Decoration::Color::Black, 12, Decoration::Dot, 7, Decoration::Start, 1, Decoration::Exit, 2);
	//Tetris Vault
	generator->resetConfig();
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->setGridSize(6, 6);
	generator->generate(0x0CC7B, Decoration::Dot_Intersection, 49, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Orange, 4, Decoration::Poly | Decoration::Negative, 3);
	//Symmetry Vault
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setGridSize(7, 7);
	generator->generate(0x002A6, Decoration::Triangle | Decoration::Color::Cyan, 5, Decoration::Triangle | Decoration::Color::Yellow, 5, Decoration::Dot | Decoration::Color::Blue, 5, Decoration::Dot | Decoration::Color::Yellow, 5);
	generator->resetConfig();
	//Shipwreck Vault
	specialCase->generateSoundDotReflectionPuzzle(0x00AFB, { 7, 7 }, { DOT_MEDIUM, DOT_LARGE, DOT_MEDIUM, DOT_SMALL }, { DOT_LARGE, DOT_SMALL, DOT_MEDIUM }, 3, false);
	generator->resetConfig();
	//Jungle Vault
	specialCase->generateJungleVault(0x15ADD);
}

void PuzzleList::GenerateTrianglePanelsN()
{
	generator->setLoadingData(L"Arrows", 12);
	generator->resetConfig();
	generator->setGridSize(2, 2);
	generator->backgroundColor = { 0, 0, 0, 1 };
	generator->arrowColor = { 1, 1, 1, 1 };
	generator->generate(0x17D28, Decoration::Arrow1, 1);
	generator->generate(0x3C12B, Decoration::Arrow1, 1);
	generator->generate(0x17CF0, Decoration::Arrow1, 1);
	generator->generate(0x17FA9, Decoration::Arrow1, 1);
	generator->setGridSize(3, 3);
	generator->generate(0x17FA0, Decoration::Arrow2, 1);
	generator->generate(0x17D27, Decoration::Arrow2, 1);
	generator->generate(0x17CFB, Decoration::Arrow2, 1);
	generator->generate(0x17D01, Decoration::Arrow2, 1);
	generator->setGridSize(4, 4);
	generator->generate(0x17C71, Decoration::Arrow3, 1);
	generator->generate(0x17CF7, Decoration::Arrow3, 1);
	generator->generate(0x17C42, Decoration::Arrow3, 1);
	generator->generate(0x17CE7, Decoration::Arrow3, 1);
}

void PuzzleList::GenerateMountainN()
{
	std::wstring text = L"Mountain Perspective";
	SetWindowText(_handle, text.c_str());
	specialCase->generateMountaintop(0x17C34, { { Decoration::Decoration::Triangle | Decoration::Color::Green, 5} });

	generator->setLoadingData(L"Mountain", 39);
	generator->resetConfig();
	//Purple Bridge
	generator->setFlagOnce(Generate::Config::PreserveStructure);
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	std::set<Point> bpoints1 = { { 6, 3 },{ 5, 4 },{ 7, 4 } };
	std::set<Point> bpoints2 = { { 6, 5 },{ 5, 6 },{ 6, 7 },{ 7, 6 } };
	std::set<Point> bpoints3 = { { 3, 6 },{ 4, 7 } };
	if (Random::rand() % 2 == 0) generator->hitPoints = { generator->pick_random(bpoints1), generator->pick_random(bpoints2), generator->pick_random(bpoints3) };
	else generator->hitPoints = { generator->pick_random(bpoints3), generator->pick_random(bpoints2), generator->pick_random(bpoints1) };
	generator->setObstructions({ { 4, 1 },{ 6, 1 },{ 8, 1 } });
	generator->blockPos = { { 1, 1 },{ 11, 1 },{ 1, 11 },{ 11, 11 } };
	generator->setSymbol(Decoration::Gap_Row, 3, 4);
	generator->generate(0x09E39, Decoration::Stone | Decoration::Color::Black, 4, Decoration::Stone | Decoration::Color::White, 4,
		Decoration::Stone | Decoration::Color::Purple, 4, Decoration::Eraser | Decoration::Color::White, 1);

	//Orange Row
	generator->resetConfig();
	generator->setGridSize(5, 5);
	generator->generate(0x09E73, Decoration::Arrow, 2, Decoration::Dot, 13);
	generator->generate(0x09E75, Decoration::Arrow, 2, Decoration::Stone | Decoration::Color::Black, 5, Decoration::Stone | Decoration::Color::White, 5);
	generator->generate(0x09E78, Decoration::Arrow, 2, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::White, 6);
	generator->generate(0x09E79, Decoration::Arrow, 2, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Green, 2, Decoration::Poly | Decoration::Color::Green, 1, Decoration::Gap, 9);
	generator->generate(0x09E6C, Decoration::Arrow, 2, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::White, 4, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2);
	generator->generate(0x09E6F, Decoration::Start, 1, Decoration::Arrow, 3, Decoration::Dot, 36);
	generator->generate(0x09E6B, Decoration::Start, 1, Decoration::Arrow, 4, Decoration::Dot, 36);
	//Green Row
	generator->setGridSize(4, 4);
	generator->generate(0x09E7A, Decoration::Dot, 5, Decoration::Stone | Decoration::Color::Black, 6, Decoration::Stone | Decoration::Color::White, 4);
	generator->generate(0x09E71, Decoration::Stone | Decoration::Color::White, 4, Decoration::Stone | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 3, Decoration::Dot, 5);
	generator->generate(0x09E72, Decoration::Poly | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::White, 3, Decoration::Stone | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 2);
	generator->generate(0x09E69, Decoration::Eraser | Decoration::Color::White, 1, Decoration::Stone | Decoration::Color::White, 3, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 3);
	generator->generate(0x09E7B, Decoration::Triangle | Decoration::White, 5, Decoration::Dot, 25);
	//Purple Panels
	generator->setGridSize(5, 5);
	generator->generate(0x09EAD, Decoration::Arrow, 3, Decoration::Triangle, 4);
	generator->generate(0x09EAF, Decoration::Arrow, 4, Decoration::Triangle, 3);
	generator->resetConfig();
	//Blue Row
	generator->setObstructions({ { 4, 3 },{ 5, 4 },{ 5, 6 },{ 5, 8 },{ 5, 10 },{ 6, 9 },{ 7, 10 } });
	generator->generate(0x33AF5, Decoration::Triangle, 7);
	if (Random::rand() % 2 == 0) generator->setObstructions({ { 5, 4 },{ 5, 6 },{ 5, 8 },{ 5, 10 },{ 9, 4 },{ 9, 6 },{ 9, 8 },{ 9, 10 },{ 7, 0 },{ 7, 2 } });
	else generator->setObstructions({ { 3, 4 },{ 3, 6 },{ 3, 8 },{ 3, 10 },{ 7, 4 },{ 7, 6 },{ 7, 8 },{ 7, 10 },{ 5, 0 },{ 5, 2 },{ 9, 0 },{ 9, 2 } });
	generator->generate(0x33AF7, Decoration::Triangle, 7);
	generator->setObstructions({ { 0, 1 },{ 0, 3 },{ 0, 5 },{ 0, 7 },{ 9, 4 },{ 1, 4 },{ 1, 6 },{ 1, 8 },{ 2, 7 },{ 2, 9 },{ 3, 8 },{ 3, 10 },{ 4, 9 },{ 5, 8 },{ 5, 10 },
		{ 6, 7 },{ 6, 9 },{ 7, 6 },{ 7, 8 },{ 7, 10 },{ 8, 5 },{ 8, 7 },{ 8, 9 },{ 9, 2 },{ 9, 4 },{ 9, 6 },{ 9, 8 },{ 10, 1 },{ 10, 3 },{ 10, 5 } });
	generator->blockPos = { { 5, 0 }, { 6, 0 }, { 4, 2 }, { 5, 2 }, { 4, 3 }, { 3, 4 } };
	generator->generate(0x09F6E, Decoration::Triangle, 6);

	//Rainbow Row
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteColors);
	generator->generate(0x09FD3, Decoration::Triangle | Decoration::Color::Magenta, 2, Decoration::Triangle | Decoration::Color::Green, 2,
		Decoration::Stone | Decoration::Color::Magenta, 3, Decoration::Stone | Decoration::Color::Green, 3);
	generator->generate(0x09FD4, Decoration::Triangle | Decoration::Color::Magenta, 2, Decoration::Triangle | Decoration::Color::Green, 3,
		Decoration::Star | Decoration::Color::Magenta, 3, Decoration::Star | Decoration::Color::Green, 3, Decoration::Dot, 3);
	generator->setGridSize(5, 5);
	generator->generate(0x09FD6, Decoration::Eraser | Decoration::Color::Cyan, 1, Decoration::Eraser | Decoration::Color::Yellow, 1, 
		Decoration::Stone | Decoration::Color::Cyan, 4, Decoration::Stone | Decoration::Color::Yellow, 4,
		Decoration::Triangle | Decoration::Color::Cyan, 2, Decoration::Triangle | Decoration::Color::Yellow, 2);
	generator->generate(0x09FD7, Decoration::Eraser | Decoration::Color::Green, 1, Decoration::Eraser | Decoration::Color::Magenta, 1,
		Decoration::Star | Decoration::Color::Green, 3, Decoration::Star | Decoration::Color::Magenta, 3,
		Decoration::Triangle | Decoration::Color::Green, 2, Decoration::Triangle | Decoration::Color::Magenta, 2);
	generator->setFlagOnce(Generate::Config::Write2Color);
	generator->setSymbol(Decoration::Start, 10, 0);
	generator->setSymbol(Decoration::Start, 0, 10);
	generator->setSymbol(Decoration::Exit, 0, 0);
	generator->setSymbol(Decoration::Exit, 10, 10);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(0x09FD8, Decoration::Triangle, 8);
	generator->resetConfig();

	specialCase->generateMultiPuzzle({ 0x09FCC, 0x09FCE, 0x09FCF, 0x09FD0, 0x09FD1, 0x09FD2 }, {
		{{ Decoration::Stone | Decoration::Color::Black, 3 },{ Decoration::Stone | Decoration::Color::White, 2 } },
		{ { Decoration::Triangle | Decoration::Color::Orange, 2 } },
		{ { Decoration::Star | Decoration::Color::White, 2 },{ Decoration::Star | Decoration::Color::Orange, 2 },{ Decoration::Star | Decoration::Color::Magenta, 2 },{ Decoration::Star | Decoration::Color::Green, 2 } },
		{ { Decoration::Stone | Decoration::Color::Cyan, 1 },{ Decoration::Stone | Decoration::Color::Yellow, 1 },{ Decoration::Star | Decoration::Color::Cyan, 1 },{ Decoration::Star | Decoration::Color::Yellow, 1 } },
		{ { Decoration::Dot_Intersection, 5 } },
		{ { Decoration::Poly, 3 }} }, false);

	specialCase->generate2Bridge(0x09E86, 0x09ED8);

	specialCase->generateMountainFloor();

	//Pillar Puzzles
	generator->setGridSize(6, 3);
	generator->generate(0x0383D, Decoration::Triangle, 5);
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteColors);	
	generator->setFlag(Generate::Config::DecorationsOnly);
	generator->generate(0x0383A, Decoration::Star | Decoration::Color::Orange, 6, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Green, 4);
	generator->generate(0x0383F, Decoration::Stone | Decoration::Color::Black, 3, Decoration::Stone | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::White, 2);
	generator->removeFlag(Generate::Config::DecorationsOnly);
	generator->generate(0x09E56, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Dot, 5);
	generator->removeFlag(Generate::Config::WriteColors);
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(0x03859, Decoration::Poly, 3);
	generator->setSymbol(Decoration::Start, 4, 8);
	generator->generate(0x09E5A, Decoration::Dot_Intersection, 30, Decoration::Triangle, 5);
	generator->setFlagOnce(Generate::Config::WriteColors);
	generator->setSymmetry(Panel::Symmetry::PillarRotational);
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	generator->generate(0x339BB, Decoration::Triangle | Decoration::Color::Orange, 6);
	generator->setSymmetry(Panel::Symmetry::PillarParallel);
	generator->generate(0x33961, Decoration::Triangle, 5, Decoration::Dot, 7);
}

void PuzzleList::GenerateCavesN()
{
	generator->setLoadingData(L"Caves", 51);
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->generate(0x17FA2, Decoration::Arrow, 6);
	generator->generate(0x00FF8, Decoration::Arrow, 2, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2,
		Decoration::Triangle | Decoration::Color::Orange, 3);

	//Blue Symmetry/Tetris
	generator->setFlag(Generate::Config::RequireCombineShapes);
	generator->setGridSize(5, 5);
	generator->generate(0x01A0D, Decoration::Arrow, 3, Decoration::Triangle | Decoration::Color::Orange, 5);
	generator->setGridSize(5, 5);
	generator->generate(0x018A0, Decoration::Poly, 3, Decoration::Arrow, 3);
	generator->resetConfig();
	generator->setFlag(Generate::Config::DisconnectShapes);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x009A4, Decoration::Poly, 3, Decoration::Triangle | Decoration::Color::Orange, 3);
	generator->generate(0x00A72, Decoration::Poly, 2, Decoration::Poly | Decoration::Negative, 2, Decoration::Triangle | Decoration::Color::Orange, 2);
	generator->resetConfig();

	//Triangle Puzzles
	generator->setFlag(Generate::Config::WriteDotColor);
	generator->generate(0x00190, Decoration::Dot, 25, Decoration::Arrow, 3, Decoration::Triangle, 2);
	generator->generate(0x00558, Decoration::Dot, 25, Decoration::Arrow, 3, Decoration::Triangle, 3);
	generator->generate(0x00567, Decoration::Dot, 25, Decoration::Arrow, 4, Decoration::Triangle, 3);
	generator->generate(0x006FE, Decoration::Dot, 25, Decoration::Arrow, 4, Decoration::Triangle, 4);
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->generate(0x008B8, Decoration::Stone | Decoration::Color::Magenta, 4, Decoration::Stone | Decoration::Color::Cyan, 4, Decoration::Triangle | Decoration::Color::Orange, 3);
	generator->generate(0x00973, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Cyan, 4, Decoration::Triangle | Decoration::Color::Orange, 3);
	generator->generate(0x0097B, Decoration::Stone | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Cyan, 2, 
		Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::Cyan, 2, Decoration::Triangle | Decoration::Color::Orange, 3);
	generator->generate(0x0097D, Decoration::Eraser | Decoration::Color::Cyan, 1, Decoration::Stone | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Cyan, 2,
		Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::Cyan, 2, Decoration::Triangle | Decoration::Color::Orange, 3);
	generator->generate(0x0097E, Decoration::Poly | Decoration::Can_Rotate, 4, Decoration::Triangle | Decoration::Color::Orange, 3);

	generator->setGridSize(4, 4);
	generator->generate(0x00994, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Triangle | Decoration::Color::Orange, 1);
	generator->generate(0x334D5, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Triangle | Decoration::Color::Orange, 2);
	generator->generate(0x00995, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Triangle | Decoration::Color::Orange, 3);
	generator->generate(0x00996, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Triangle | Decoration::Color::Orange, 4);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x00998, Decoration::Poly, 3, Decoration::Triangle | Decoration::Color::Orange, 5);
	generator->resetConfig();

	//First alcove
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(0x32962, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Dot, 7);
	generator->setFlagOnce(Generate::Config::WriteColors);
	generator->generate(0x32966, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 2,
		Decoration::Poly | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2, Decoration::Triangle | Decoration::Color::Black, 1, Decoration::Triangle | Decoration::Color::White, 1);
	generator->setFlagOnce(Generate::Config::WriteColors);
	generator->setGridSize(6, 6);
	generator->generate(0x01A31, Decoration::Stone | Decoration::Cyan, 9, Decoration::Stone | Decoration::Yellow, 8, Decoration::Stone | Decoration::Magenta, 7);
	generator->setFlagOnce(Generate::Config::AlternateColors);
	generator->setGridSize(5, 5);
	generator->generate(0x00B71, Decoration::Eraser | Decoration::Color::Black, 1, Decoration::Eraser | Decoration::Color::White, 1, 
		Decoration::Dot, 3, Decoration::Stone | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::White, 1, Decoration::Star | Decoration::Color::Black, 1, Decoration::Star| Decoration::Color::White, 1,
		Decoration::Poly | Decoration::Color::Black, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 2);
	generator->resetConfig();

	//Perspective	
	generator->setFlag(Generate::Config::DecorationsOnly);
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(0x288EA, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Poly | Decoration::Negative, 1);
	generator->generate(0x288FC, Decoration::Star | Decoration::Color::Black, 6, Decoration::Star | Decoration::Color::White, 10);
	generator->generate(0x289E7, Decoration::Poly, 2, Decoration::Poly | Decoration::Can_Rotate, 1,
		Decoration::Star | Decoration::Color::Black, 4, Decoration::Stone | Decoration::Color::White, 5);
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->generate(0x288AA, Decoration::Poly, 2, Decoration::Star | Decoration::Color::Black, 4, Decoration::Stone | Decoration::Color::White, 4, Decoration::Star | Decoration::Color::Magenta, 4);
	generator->resetConfig();
	
	//Full Dots
	generator->setFlag(Generate::Config::EnableFlash);
	generator->backgroundColor = { 0, 0, 0, 1 };
	generator->successColor = { 1, 1, 1, 1 };
	generator->setGridSize(4, 4);
	generator->generate(0x0A16B, Decoration::Start, 1, Decoration::Dot_Intersection, 25, Decoration::Stone | Decoration::Color::Black, 4, Decoration::Stone | Decoration::Color::White, 3);
	generator->generate(0x0A2CE, Decoration::Start, 1, Decoration::Dot_Intersection, 25, Decoration::Star | Decoration::Color::Black, 6, Decoration::Star | Decoration::Color::White, 4);
	generator->generate(0x0A2D7, Decoration::Start, 1, Decoration::Dot_Intersection, 25, Decoration::Poly | Decoration::Color::Black, 3);
	generator->generate(0x0A2DD, Decoration::Start, 1, Decoration::Dot_Intersection, 36, Decoration::Triangle | Decoration::Color::Black, 6);
	generator->generate(0x0A2EA, Decoration::Start, 1, Decoration::Dot_Intersection, 36, Decoration::Triangle | Decoration::Color::Black, 7, Decoration::Eraser | Decoration::Color::Black, 1);
	generator->setGridSize(5, 5);
	generator->generate(0x17FB9, Decoration::Start, 1, Decoration::Dot_Intersection, 36, Decoration::Star | Decoration::Color::Black, 6, Decoration::Star | Decoration::Color::White, 4);
	generator->resetConfig();

	//Invisible Dots
	generator->setFlag(Generate::Config::WriteColors);
	generator->setGridSize(4, 4);
	generator->generate(0x0008F, Decoration::Dot, 9);
	generator->generate(0x0006B, Decoration::Stone | Decoration::Color::Black, 4, Decoration::Stone | Decoration::Color::X, 6);
	generator->generate(0x0008B, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::X, 2, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::X, 2);
	generator->generate(0x0008C, Decoration::Poly | Decoration::Color::X, 2, Decoration::Poly | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::X, 4);
	generator->setGridSize(5, 5);
	generator->generate(0x0008A, Decoration::Stone | Decoration::Color::X, 4, Decoration::Stone | Decoration::Color::Black, 3 , Decoration::Star | Decoration::Color::X, 2, Decoration::Star | Decoration::Color::Black, 4);
	generator->generate(0x00089, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::X, 1, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Black, 2,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::X, 2, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::X, 2);
	generator->setFlag(Generate::Config::RequireCancelShapes);
	generator->generate(0x0006A, Decoration::Poly | Decoration::Color::X, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::X, 5, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::X, 4);
	generator->generate(0x0006C, Decoration::Eraser | Decoration::Color::X, 1, Decoration::Dot, 5, Decoration::Poly | Decoration::Color::X, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::X, 6);
	generator->setGridSize(3, 3);
	generator->generate(0x00027, Decoration::Stone | Decoration::Color::X, 2, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Eraser | Decoration::Color::X, 1);
	generator->setGridSize(5, 5);
	generator->generate(0x00028, Decoration::Stone | Decoration::Color::X, 6, Decoration::Stone | Decoration::Color::Black, 6, Decoration::Eraser | Decoration::Color::X, 1);
	generator->setGridSize(5, 5);
	generator->generate(0x00029, Decoration::Star | Decoration::Black, 6, Decoration::Star | Decoration::X, 5, Decoration::Eraser | Decoration::Color::X, 1);
	generator->resetConfig();
	generator->setFlag(Generate::Config::EnableFlash);
	//Triangle Exit
	generator->setGridSize(5, 5);
	generator->generate(0x17CF2, Decoration::Triangle1 | Decoration::Color::Magenta, 15);
	//Stars Exit
	generator->setGridSize(4, 4);
	generator->generate(0x021D7, Decoration::Triangle3, 7);

	//Challenge Pillar
	generator->setGridSize(5, 5);
	generator->generate(0x09DD5, Decoration::Triangle | Decoration::Color::Orange, 8, Decoration::Dot, 8);
	generator->resetConfig();
	//Challenge Entrance
	generator->generate(0x0A16E, Decoration::Triangle | Decoration::Color::Green, 3, Decoration::Star | Decoration::Color::Green, 5, Decoration::Poly | Decoration::Color::Green, 3);
	//Theater Exit
	generator->setGridSize(4, 4);
	generator->generate(0x039B4, Decoration::Triangle2 | Decoration::Color::Orange, 7, Decoration::Dot, 9);
	//Town Exit
	generator->setGridSize(6, 6);
	generator->generate(0x09E85, Decoration::Triangle | Decoration::Color::Orange, 11, Decoration::Dot, 11, Decoration::Start, 3, Decoration::Exit, 1);
}

void PuzzleList::GenerateOrchardN()
{
	specialCase->generateApplePuzzle(0x032FF, true, false);
}

void PuzzleList::GenerateDesertN()
{
	Randomizer().RandomizeDesert();
}

void PuzzleList::GenerateKeepN()
{
	generator->setLoadingData(L"Keep", 5);
	generator->resetConfig();

	generator->setSymbol(Decoration::Gap_Column, 8, 3);
	generator->setSymbol(Decoration::Gap_Column, 4, 5);
	generator->setSymbol(Decoration::Gap_Row, 3, 0);
	generator->setSymbol(Decoration::Gap_Row, 3, 2);
	generator->setSymbol(Decoration::Gap_Row, 5, 6);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->setObstructions({ { 1, 4 },{ 2, 3 },{ 5, 4 },{ 5, 8 } });
	generator->generate(0x033EA, Decoration::Triangle, 5);
	std::set<Point> path1 = generator->_path;
	std::vector<std::vector<Point>> sets = { { { 7, 8 },{ 8, 7 },{ 7, 6 },{ 6, 7 } },{ { 6, 5 },{ 7, 4 },{ 8, 5 } },{ { 7, 0 },{ 7, 2 },{ 6, 1 },{ 8, 1 },{ 5, 2 } },
	{ { 2, 7 },{ 4, 7 },{ 3, 8 },{ 3, 6 },{ 1, 6 } },{ { 0, 1 },{ 1, 0 },{ 2, 1 },{ 1, 2 } } };
	for (std::vector<Point> set : sets) {
		Point p = pick_random(set);
		while (!path1.count(p)) p = pick_random(set);
		generator->set(p, p.first % 2 == 0 ? Decoration::Dot_Column : Decoration::Dot_Row);
	}
	generator->write(0x033EA);

	generator->resetConfig();
	generator->setObstructions({ { 3, 2 },{ 8, 5 } });
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(0x01BE9, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::White, 4,
		Decoration::Triangle | Decoration::Color::Black, 2, Decoration::Triangle | Decoration::Color::White, 2);
	std::set<Point> path2 = generator->_path;
	generator->write(0x01BE9);

	generator->resetConfig();
	std::vector<std::vector<Point>> validHitPoints = {
		{ { 5, 8 },{ 3, 4 },{ 7, 2 },{ 3, 2 } },{ { 8, 7 },{ 7, 2 },{ 6, 7 },{ 5, 8 } },{ { 5, 8 },{ 4, 5 },{ 7, 2 },{ 2, 5 } },
	{ { 5, 8 },{ 3, 6 },{ 7, 2 },{ 3, 4 } },{ { 5, 8 },{ 1, 6 },{ 7, 2 },{ 1, 4 } },{ { 5, 8 },{ 4, 3 },{ 7, 2 },{ 2, 3 } },
	{ { 5, 8 },{ 3, 4 },{ 7, 2 },{ 3, 2 } },{ { 5, 8 },{ 1, 4 },{ 7, 2 },{ 1, 2 } },{ { 5, 8 },{ 3, 2 },{ 7, 2 },{ 3, 0 } },
	{ { 5, 8 },{ 1, 2 },{ 7, 2 },{ 1, 0 } } };
	generator->hitPoints = validHitPoints[Random::rand() % validHitPoints.size()];
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(0x01CD3, Decoration::Poly, 2, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::Cyan, 2);
	std::set<Point> path3 = generator->_path;
	generator->write(0x01CD3);

	generator->resetConfig();
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(0x01D3F, Decoration::Poly | Decoration::Can_Rotate, 3);
	if (generator->_panel->_endpoints[0].GetX() == 0) {
		std::swap(generator->_panel->_endpoints[0], generator->_panel->_endpoints[1]); //Need to have endpoints in right order to associate with pressure plates correctly
	}
	std::set<Point> path4 = (generator->_path1.count(Point(0, 8)) ? generator->_path2 : generator->_path1);
	if (generator->_path.count({ 7, 0 })) generator->set(7, 0, Decoration::Dot_Row);
	else generator->set(8, 1, Decoration::Dot_Column);
	generator->write(0x01D3F);

	specialCase->generateKeepLaserPuzzle(0x03317, path1, path2, path3, path4,
		{ { Decoration::Dot, 7 },{ Decoration::Star | Decoration::Color::Black, 4 },{ Decoration::Star | Decoration::Color::White, 4 }, { Decoration::Star | Decoration::Color::Magenta, 4 }, { Decoration::Star | Decoration::Color::Cyan, 4 },
		{ Decoration::Stone | Decoration::Color::Black, 5 },{ Decoration::Stone | Decoration::Color::White, 4 },{ Decoration::Stone | Decoration::Color::Cyan, 5 },
		{ Decoration::Stone | Decoration::Color::Magenta, 4 },{ Decoration::Poly, 5 } });

	specialCase->clearTarget(0x0360E); //Must solve pressure plate side
}

void PuzzleList::GenerateJungleN()
{
	//Can't random sound cues just yet. I found the file names in memory, but have yet to figure out where the actual sound files are.
	//specialCase->testFind({ 'b', 'i', 'r', 'd', '3' }); //0x52a230
	//specialCase->testFind({ 'b', 'i', 'r', 'd', '4' }); //0x52a2d8
	//So the only thing that can be randomized currently, are the actual solutions.

	generator->setLoadingData(L"Jungle", 4);
	generator->resetConfig();

	specialCase->generateSoundDotPuzzle(0x0026F, { 4, 4 }, { DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_MEDIUM, DOT_LARGE }, false);
	if (Random::rand() % 2) specialCase->generateSoundDotPuzzle(0x00C3F, { 4, 4 }, { DOT_SMALL, DOT_MEDIUM, DOT_SMALL, DOT_LARGE }, true);
	else specialCase->generateSoundDotPuzzle(0x00C3F, { 4, 4 }, { DOT_LARGE, DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_LARGE }, true);
	if (Random::rand() % 2) specialCase->generateSoundDotPuzzle(0x00C41, { 4, 4 }, { DOT_SMALL, DOT_SMALL, DOT_LARGE, DOT_MEDIUM, DOT_LARGE }, true);
	else specialCase->generateSoundDotPuzzle(0x00C41, { 4, 4 }, { DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_MEDIUM, DOT_LARGE }, true);
	if (Random::rand() % 2) specialCase->generateSoundDotPuzzle(0x014B2, { 4, 4 }, { DOT_SMALL, DOT_LARGE, DOT_SMALL, DOT_LARGE, DOT_MEDIUM }, true);
	else specialCase->generateSoundDotPuzzle(0x014B2, { 4, 4 }, { DOT_LARGE, DOT_MEDIUM, DOT_SMALL, DOT_LARGE, DOT_SMALL }, true);
}

//--------------------------HARD MODE-----------------------------

void PuzzleList::GenerateTutorialH()
{
	generator->setLoadingData(L"Tutorial", 21);
	generator->resetConfig();
	Special::drawSeedAndDifficulty(0x00064, seed, true, !seedIsRNG, false);
	Special::drawGoodLuckPanel(0x00182);
	generator->setFlag(Generate::Config::WriteDotColor2);
	generator->setFlag(Generate::Config::LongestPath);
	//Mazes
	generator->setFlag(Generate::Config::FullGaps);
	generator->setGridSize(5, 5);
	generator->generate(0x00293, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Stone | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::Black, 5, Decoration::Dot, 3, Decoration::Poly | Decoration::Color::Black, 2);
	generator->setGridSize(6, 6);
	generator->generate(0x00295, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Triangle | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::Black, 5, Decoration::Poly | Decoration::Color::Black, 2);
	generator->setGridSize(7, 7);
	generator->generate(0x002C2, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Stone | Decoration::Color::Black, 5, Decoration::Triangle| Decoration::Color::Black, 7, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Black, 3);
	generator->removeFlag(Generate::Config::FullGaps);
	//2 starts maze
	generator->setGridSize(6, 6);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(0x0A3B2, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Stone | Decoration::Color::Black, 5, Decoration::Triangle | Decoration::Color::Black, 5, Decoration::Star | Decoration::Color::Black, 5, Decoration::Dot, 49);
	generator->set(0, 9, 0); generator->set(1, 10, 0); generator->set(9, 10, 0); generator->set(10, 9, 0);
	generator->write(0x0A3B2);
	//2 exits maze
	generator->setGridSize(5, 5);
	generator->setSymbol(Decoration::Start, 0, 0);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(0x0A3B5, Decoration::Dot_Intersection, 36, Decoration::Gap, 12);
	generator->set(9, 0, 0); generator->set(10, 1, 0); generator->set(9, 10, 0); generator->set(10, 9, 0);
	generator->write(0x0A3B5);
	generator->resetConfig();
	//Exit Gate
	specialCase->modifyGate(0x03629);
	//Secret back area
	generator->setGridSize(4, 4);
	generator->setSymbol(Decoration::Start, 8, 0);
	generator->setSymbol(Decoration::Exit, 0, 0);
	generator->generate(0x0A171, Decoration::Dot_Intersection, 25, Decoration::Arrow, 2, Decoration::Triangle | Decoration::Color::Orange, 3);
	generator->setGridSize(5, 5);
	generator->setSymbol(Decoration::Start, 10, 0);
	generator->setSymbol(Decoration::Exit, 0, 10);
	generator->setFlagOnce(Generate::Config::FalseParity);
	generator->generate(0x04CA4, Decoration::Dot_Intersection, 36, Decoration::Arrow, 3, Decoration::Triangle | Decoration::Color::Orange, 4);
	//Dots Tutorial
	generator->setGridSize(4, 4);
	generator->generate(0x0005D, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot_Intersection, 25, Decoration::Arrow, 1, Decoration::Stone | Decoration::Color::Black, 3, Decoration::Stone | Decoration::Color::White, 3);
	generator->generate(0x0005E, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot_Intersection, 25, Decoration::Arrow, 2, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star| Decoration::Color::White, 2);
	generator->generate(0x0005F, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot_Intersection, 25, Decoration::Arrow, 3, Decoration::Poly, 2, Decoration::Poly | Decoration::Negative, 2);
	generator->setGridSize(5, 5);
	generator->generate(0x00060, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot_Intersection, 36, Decoration::Arrow, 3, Decoration::Triangle | Decoration::Color::Orange, 3);
	generator->generate(0x00061, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot_Intersection, 36, Decoration::Arrow, 4, Decoration::Triangle | Decoration::Color::Orange, 4);
	//Stones Tutorial
	generator->resetConfig();
	generator->setGridSize(3, 3);
	generator->generate(0x018AF, Decoration::Arrow1, 1, Decoration::Triangle1, 1);
	generator->generate(0x0001B, Decoration::Arrow2, 1, Decoration::Triangle2, 1);
	generator->generate(0x012C9, Decoration::Arrow3, 1, Decoration::Triangle3, 1);
	generator->setGridSize(4, 4);
	generator->generate(0x0001C, Decoration::Arrow, 2, Decoration::Triangle, 2);
	generator->generate(0x0001D, Decoration::Arrow, 2, Decoration::Triangle, 3);
	generator->generate(0x0001E, Decoration::Arrow, 3, Decoration::Triangle, 3);
	generator->setGridSize(5, 5);
	generator->generate(0x0001F, Decoration::Arrow, 3, Decoration::Triangle, 4);
	generator->generate(0x00020, Decoration::Arrow, 4, Decoration::Triangle, 3);
	generator->generate(0x00021, Decoration::Arrow, 4, Decoration::Triangle, 4);
}

void PuzzleList::GenerateSymmetryH()
{
	generator->setLoadingData(L"Symmetry", 34);
	generator->resetConfig();
	//Symmetry Mazes
	generator->setFlag(Generate::Config::WriteDotColor2);
	generator->setSymmetry(Panel::Symmetry::ParallelH);
	generator->setGridSize(7, 7);
	generator->pathWidth = 0.8f;
	generator->generate(0x00086, Decoration::Dot, 11, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setSymmetry(Panel::Symmetry::ParallelHFlip);
	generator->setGridSize(7, 7);
	generator->pathWidth = 0.8f;
	generator->generate(0x00087, Decoration::Dot, 13, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setSymmetry(Panel::Symmetry::ParallelV);
	generator->setGridSize(7, 7);
	generator->pathWidth = 0.8f;
	generator->generate(0x00059, Decoration::Dot, 15, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setSymmetry(Panel::Symmetry::ParallelVFlip);
	generator->setGridSize(7, 7);
	generator->pathWidth = 0.8f;
	generator->generate(0x00062, Decoration::Dot, 17, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setGridSize(11, 8);
	generator->pathWidth = 1;
	generator->setSymbol(Decoration::Start, 0, 8);
	generator->setSymbol(Decoration::Start, 22, 8);
	generator->setSymbol(Decoration::Exit, 0, 6);
	generator->setSymbol(Decoration::Exit, 22, 10);
	generator->setObstructions({ { 0, 15 },{ 1, 16 },{ 11, 16 },{ 12, 15 },{ 13, 16 } });
	generator->hitPoints = { { 22, 7 },{ 22, 5 },{ 22, 3 },{ 22, 1 },{ 21, 0 },{ 10, 1 } };
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(0x0005C, Decoration::Gap, 30, Decoration::Dot_Intersection, 7);
	generator->set(12, 16, Decoration::Dot_Intersection);
	generator->set(0, 16, Decoration::Dot_Intersection);
	generator->write(0x0005C);
	//Weird Symmetry 1
	generator->setGridSize(7, 7);
	generator->pathWidth = 0.8f;
	generator->setSymmetry(Panel::Symmetry::Horizontal);
	generator->generate(0x0008D, Decoration::Dot, 13, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Gap, 11);
	generator->setSymmetry(Panel::Symmetry::Vertical);
	generator->generate(0x00081, Decoration::Dot, 15, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Gap, 11);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(0x00083, Decoration::Dot, 17, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Gap, 11);
	//Weird Symmetry 2
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x00084, Decoration::Dot, 13);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x00082, Decoration::Stone | Decoration::Color::Magenta, 5, Decoration::Stone | Decoration::Color::White, 5);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x0343A, Decoration::Star | Decoration::Color::Magenta, 6, Decoration::Star | Decoration::Color::White, 4);
	generator->resetConfig();
	//Symmetry Island Door
	generator->setGridSize(5, 5);
	generator->generate(0x000B0, Decoration::Triangle | Decoration::Color::Cyan, 9);
	//Triangle Symmetry
	generator->setGridSize(7, 7);
	generator->setSymmetry(Panel::Symmetry::RotateLeft);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x00022, Decoration::Triangle4 | Decoration::Color::Yellow, 1, Decoration::Triangle | Decoration::Color::Yellow, 4);
	generator->setSymmetry(Panel::Symmetry::RotateRight);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x00023, Decoration::Triangle4 | Decoration::Color::Yellow, 1, Decoration::Triangle | Decoration::Color::Yellow, 6);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x00024, Decoration::Triangle | Decoration::Color::Yellow, 9);
	generator->setSymmetry(Panel::Symmetry::RotateRight);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x00025, Decoration::Triangle4 | Decoration::Color::Yellow, 1, Decoration::Triangle | Decoration::Color::Yellow, 6);
	generator->setSymmetry(Panel::Symmetry::RotateLeft);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x00026, Decoration::Triangle4 | Decoration::Color::Yellow, 1, Decoration::Eraser, 1, Decoration::Triangle | Decoration::Color::Yellow, 8);
	//2-color Mechanics
	generator->setFlag(Generate::Config::WriteInvisible);
	generator->setSymmetry(Panel::Symmetry::ParallelH);
	generator->generate(0x0007C, Decoration::Eraser, 1, Decoration::Dot | Decoration::Color::Blue, 4, Decoration::Dot | Decoration::Color::Yellow, 4, Decoration::Dot, 8, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setSymmetry(Panel::Symmetry::ParallelHFlip);
	generator->generate(0x0007E, Decoration::Eraser, 1, Decoration::Stone | Decoration::Color::Cyan, 8, Decoration::Stone | Decoration::Color::Yellow, 8, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setSymmetry(Panel::Symmetry::ParallelV);
	generator->generate(0x00075, Decoration::Eraser | Decoration::Color::Cyan, 1, Decoration::Star | Decoration::Color::Cyan, 7, Decoration::Star | Decoration::Color::Yellow, 6, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setSymmetry(Panel::Symmetry::ParallelVFlip);
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->generate(0x00073, Decoration::Eraser, 1, Decoration::Poly | Decoration::Color::Yellow, 5, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setFlag(Generate::Config::ResetColors);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(0x00077, Decoration::Eraser, 1, Decoration::Triangle | Decoration::Color::Yellow, 10, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setSymmetry(Panel::Symmetry::FlipNegXY);
	generator->generate(0x00079, Decoration::Dot | Decoration::Color::Blue, 4, Decoration::Dot | Decoration::Color::Yellow, 4, Decoration::Dot, 8,
		Decoration::Eraser | Decoration::Color::White, 1, Decoration::Start, 1);
	//Secret Symmetry
	generator->setFlag(Generate::Config::DisableDotIntersection);
	std::vector<int> ids = { 0x00065, 0x0006D, 0x00072, 0x0006F, 0x00070, 0x00071 };
	std::vector<Panel::Symmetry> sym1 = { Panel::Symmetry::Vertical, Panel::Symmetry::Horizontal, Panel::Symmetry::Rotational, Panel::Symmetry::ParallelH, Panel::Symmetry::ParallelV };
	std::vector<Panel::Symmetry> sym2 = { Panel::Symmetry::ParallelHFlip, Panel::Symmetry::ParallelVFlip, Random::rand() % 2 == 0 ? Panel::Symmetry::ParallelV : Panel::Symmetry::ParallelH };
	Panel::Symmetry lastChoice = Panel::Symmetry::None;
	for (int i = 0; i < ids.size(); i++) {
		Panel::Symmetry choice = (i < 4 ? pop_random(sym1) : pop_random(sym2));
		if (choice == lastChoice) { i--; continue; }
		lastChoice = choice;
		specialCase->initSSGrid(generator);
		generator->setSymmetry(choice);
		if (i < 5) generator->generate(ids[i], Decoration::Stone| Decoration::Color::Cyan, 4, Decoration::Stone | Decoration::Color::Yellow, 4, Decoration::Dot | Decoration::Color::Cyan, 2, Decoration::Dot | Decoration::Color::Yellow, 2, Decoration::Triangle | Decoration::Color::Orange, 4);
		else generator->generate(ids[i], Decoration::Triangle | Decoration::Color::Orange, 8);
	}
	specialCase->initSSGrid(generator);
	generator->setSymmetry(Panel::Symmetry::None);
	generator->generate(0x00076, Decoration::Triangle | Decoration::Color::Orange, 6);
	//Symmetry Island Door 2
	specialCase->generateSymmetryGate(0x1C349);
	//Dot Reflection Dual Panels (before laser)^
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteDotColor2);
	std::set<Panel::Symmetry> normalSym = { Panel::Symmetry::Vertical, Panel::Symmetry::Rotational };
	std::set<Panel::Symmetry> weirdSym = { Panel::Symmetry::RotateLeft, Panel::Symmetry::RotateRight, Panel::Symmetry::FlipXY, Panel::Symmetry::FlipNegXY };
	generator->setSymbol(Decoration::Start, 0, 8);
	generator->setSymbol(Decoration::Exit, 8, 0);
	specialCase->generateReflectionDotPuzzle(generator, 0x00A52, 0x00A61, { { Decoration::Dot, 15 } }, pop_random(normalSym), true);
	generator->setSymbol(Decoration::Start, 0, 8);
	generator->setSymbol(Decoration::Exit, 8, 0);
	specialCase->generateReflectionDotPuzzle(generator, 0x00A57, 0x00A64, { { Decoration::Dot, 15 } }, pop_random(weirdSym), true);
	if (!weirdSym.count(Panel::Symmetry::RotateLeft)) weirdSym.erase(Panel::Symmetry::RotateRight);
	else if (!weirdSym.count(Panel::Symmetry::RotateRight)) weirdSym.erase(Panel::Symmetry::RotateLeft);
	else if (!weirdSym.count(Panel::Symmetry::FlipXY)) weirdSym.erase(Panel::Symmetry::FlipNegXY);
	else if (!weirdSym.count(Panel::Symmetry::FlipNegXY)) weirdSym.erase(Panel::Symmetry::FlipXY);
	generator->setSymbol(Decoration::Start, 0, 8); generator->setSymbol(Decoration::Start, 8, 8);
	generator->setSymbol(Decoration::Start, 8, 0); generator->setSymbol(Decoration::Start, 0, 0);
	generator->setSymbol(Decoration::Exit, 0, 4); generator->setSymbol(Decoration::Exit, 8, 4);
	generator->setFlag(Generate::Config::DisableDotIntersection);
	specialCase->generateReflectionDotPuzzle(generator, 0x00A5B, 0x00A68, { { Decoration::Dot, 15 } }, pick_random(weirdSym), true);
}

void PuzzleList::GenerateQuarryH()
{
	generator->setLoadingData(L"Quarry", 40);
	generator->resetConfig();
	//Entry Gates
	generator->setGridSize(5, 5);
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(0x09E57, Decoration::Poly, 3, Decoration::Triangle, 5);
	generator->setGridSize(5, 5);
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->generate(0x17C09, Decoration::Poly, 3, Decoration::Arrow, 2, Decoration::Gap, 11);
	//Mill Entry Door
	generator->resetConfig();
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(0x01E5A, Decoration::Arrow, 5);
	generator->setFlagOnce(Generate::Config::PreserveStructure);
	generator->generate(0x01E59, Decoration::Arrow, 7);
	//Triangles
	generator->pathWidth = 0.8f;
	generator->setFlag(Generate::Config::ResetColors);
	generator->setGridSize(4, 4);
	generator->generate(0x00E0C, Decoration::Triangle, 3, Decoration::Eraser | Decoration::Color::White, 2);
	generator->generate(0x01489, Decoration::Triangle, 5, Decoration::Eraser | Decoration::Color::White, 2);
	generator->generate(0x0148A, Decoration::Triangle, 7, Decoration::Eraser | Decoration::Color::White, 2);
	generator->setGridSize(5, 5);
	generator->generate(0x014D9, Decoration::Triangle, 9, Decoration::Eraser | Decoration::Color::White, 2);
	generator->generate(0x014E7, Decoration::Triangle, 11, Decoration::Eraser | Decoration::Color::White, 2);
	generator->generate(0x014E8, Decoration::Triangle, 13, Decoration::Eraser | Decoration::Color::White, 2);
	generator->removeFlag(Generate::Config::ResetColors);
	generator->pathWidth = 1;
	//Stones + Stars
	generator->setGridSize(4, 4);
	generator->setFlag(Generate::Config::AlternateColors);
	generator->generate(0x00557, Decoration::Eraser | Decoration::Color::White, 1, Decoration::Eraser | Decoration::Color::Black, 1,
		Decoration::Stone | Decoration::Color::White, 1, Decoration::Stone | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::Magenta, 1,
		Decoration::Star | Decoration::Color::White, 1, Decoration::Star | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::Magenta, 2);
	generator->generate(0x005F1, Decoration::Eraser | Decoration::Color::Magenta, 1, Decoration::Eraser | Decoration::Color::Black, 1,
		Decoration::Stone | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::Magenta, 1,
		Decoration::Star | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::Magenta, 2);
	generator->setGridSize(5, 5);
	generator->generate(0x00620, Decoration::Eraser | Decoration::Color::Magenta, 1, Decoration::Eraser | Decoration::Color::White, 1,
		Decoration::Stone | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::Magenta, 2,
		Decoration::Star | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::Magenta, 2);
	generator->generate(0x009F5, Decoration::Eraser | Decoration::Color::Black, 1, Decoration::Eraser | Decoration::Color::White, 1,
		Decoration::Stone | Decoration::Color::White, 3, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::Magenta, 3,
		Decoration::Star | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::Magenta, 3);
	generator->setGridSize(4, 4);
	generator->generate(0x0146C, Decoration::Eraser | Decoration::Color::White, 1, Decoration::Eraser | Decoration::Color::Black, 1,
		Decoration::Triangle | Decoration::Color::White, 1, Decoration::Triangle | Decoration::Color::Black, 1, Decoration::Triangle | Decoration::Color::Magenta, 1,
		Decoration::Star | Decoration::Color::White, 1, Decoration::Star | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::Magenta, 2);
	generator->generate(0x3C12D, Decoration::Eraser | Decoration::Color::Magenta, 1, Decoration::Eraser | Decoration::Color::Black, 1,
		Decoration::Triangle | Decoration::Color::White, 2, Decoration::Triangle | Decoration::Color::Black, 1, Decoration::Triangle | Decoration::Color::Magenta, 1,
		Decoration::Star | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::Magenta, 2);
	generator->setGridSize(5, 5);
	generator->generate(0x03686, Decoration::Eraser | Decoration::Color::Magenta, 1, Decoration::Eraser | Decoration::Color::White, 1,
		Decoration::Triangle | Decoration::Color::White, 2, Decoration::Triangle | Decoration::Color::Black, 2, Decoration::Triangle | Decoration::Color::Magenta, 2,
		Decoration::Star | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::Magenta, 2);
	generator->generate(0x014E9, Decoration::Eraser | Decoration::Color::Black, 1, Decoration::Eraser | Decoration::Color::White, 1,
		Decoration::Triangle | Decoration::Color::White, 2, Decoration::Triangle | Decoration::Color::Black, 3, Decoration::Triangle | Decoration::Color::Magenta, 3,
		Decoration::Star | Decoration::Color::White, 3, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::Magenta, 3);
	//Stones + Stars + Triangles
	generator->setSymbol(Decoration::Start, 0, 10);
	generator->generate(0x0367C, Decoration::Eraser | Decoration::Color::Magenta, 1, Decoration::Eraser | Decoration::Color::White, 1, 
		Decoration::Star | Decoration::Color::White, 3, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::Magenta, 2,
		Decoration::Triangle | Decoration::Color::Magenta, 2, Decoration::Triangle | Decoration::Color::Black, 3, 
		Decoration::Stone | Decoration::Color::White, 3, Decoration::Stone | Decoration::Color::Magenta, 2);
	//Optional Puzzle
	generator->resetConfig();
	generator->setFlagOnce(Generate::Config::FalseParity);
	generator->setGridSize(8, 8);
	generator->setSymbol(Decoration::Exit, 0, 0);
	generator->setSymbol(Decoration::Exit, 16, 0);
	generator->setSymbol(Decoration::Exit, 0, 16);
	generator->setSymbol(Decoration::Exit, 16, 16);
	generator->generate(0x3C125, Decoration::Star | Decoration::Color::Black, 7, Decoration::Star | Decoration::Color::White, 7,
		Decoration::Triangle | Decoration::Color::Black, 7, Decoration::Triangle | Decoration::Color::White, 7, Decoration::Dot_Intersection, 81, Decoration::Start, 5);
	//Boathouse Ramp Activation
	generator->resetConfig();
	generator->setFlag(Generate::Config::EnableFlash);
	generator->arrowColor = { 1, 1, 1, 1 };
	generator->generate(0x034D4, Decoration::Star | Decoration::Cyan, 6, Decoration::Star | Decoration::Yellow, 8, Decoration::Arrow, 3);
	generator->setGridSize(5, 5);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(0x021D5, Decoration::Poly, 3, Decoration::Arrow, 3);
	//Eraser + Shapes
	generator->setFlag(Generate::Config::ResetColors);
	generator->arrowColor = { 0, 1, 0, 1 };
	generator->setGridSize(4, 4);
	generator->generate(0x021B3, Decoration::Arrow, 2, Decoration::Poly, 3);
	generator->generate(0x021B4, Decoration::Arrow, 3, Decoration::Poly, 3);
	generator->setGridSize(5, 5);
	generator->generate(0x021B0, Decoration::Arrow, 4, Decoration::Poly, 3);
	generator->generate(0x021AF, Decoration::Arrow, 3, Decoration::Poly, 4);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);	
	generator->generate(0x021AE, Decoration::Arrow, 4, Decoration::Poly, 4);
	generator->resetConfig();
	//Eraser + Stars
	generator->setGridSize(4, 4);
	generator->arrowColor = { 0, 1, 0, 1 };
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->setFlag(Generate::Config::DisableCancelShapes);
	generator->generate(0x021B5, Decoration::Arrow, 2, Decoration::Poly | Decoration::Color::Yellow, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 2);
	generator->generate(0x021B6, Decoration::Arrow, 2, Decoration::Poly | Decoration::Color::Yellow, 3, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 2);
	generator->generate(0x021B7, Decoration::Arrow, 3, Decoration::Poly | Decoration::Color::Yellow, 4, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 3);
	generator->setGridSize(5, 5);
	generator->setFlag(Generate::Config::DisconnectShapes);
	generator->generate(0x021BB, Decoration::Arrow, 3, Decoration::Poly | Decoration::Color::Yellow, 3, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 2);
	generator->generate(0x09DB5, Decoration::Arrow, 3, Decoration::Poly | Decoration::Color::Yellow, 4, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 3);
	//Eraser + Stars + Shapes
	generator->removeFlag(Generate::Config::TreehouseColors);
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteColors);
	generator->setFlag(Generate::Config::WriteDotColor);
	generator->setGridSize(5, 5);
	generator->arrowColor = { 0, 1, 0, 1 };
	generator->generate(0x09DB1, Decoration::Arrow, 2, Decoration::Poly | Decoration::Color::Yellow, 3, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 2, Decoration::Dot_Intersection, 36, Decoration::Start, 1);
	generator->generate(0x3C124, Decoration::Arrow, 3, Decoration::Poly | Decoration::Color::Yellow, 4, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 3, Decoration::Dot_Intersection, 36, Decoration::Start, 1);
	generator->setFlag(Generate::Config::DisconnectShapes);
	generator->generate(0x09DB3, Decoration::Arrow, 2, Decoration::Poly | Decoration::Color::Yellow, 3, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 2, Decoration::Dot_Intersection, 36, Decoration::Start, 1);
	generator->generate(0x09DB4, Decoration::Arrow, 3, Decoration::Poly | Decoration::Color::Yellow, 4, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 3, Decoration::Dot_Intersection, 36, Decoration::Start, 1);
	//All together
	generator->resetConfig();
	generator->removeFlag(Generate::Config::WriteColors);
	generator->setFlag(Generate::Config::ResetColors);
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.6f;
	generator->arrowColor = { 0, 1, 1, 1 };
	generator->generate(0x0A3CB, Decoration::Arrow, 2, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Yellow, 3, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 4);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->arrowColor = { 0, 1, 1, 1 };
	generator->generate(0x0A3CC, Decoration::Arrow, 3, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Yellow, 3, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::Green, 2);
	generator->setGridSize(6, 6);
	generator->pathWidth = 0.6f;
	generator->arrowColor = { 0, 1, 1, 1 };
	generator->generate(0x0A3D0, Decoration::Arrow, 4, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Yellow, 3, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::Green, 4);
	//Laser Puzzle
	generator->resetConfig();
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	generator->initPanel(0x03612);
	generator->clear();
	generator->set(1, 5, Decoration::Eraser | Decoration::Color::White);
	generator->set(5, 5, Decoration::Eraser | Decoration::Color::Green);
	generator->set(1, 3, Decoration::Eraser | Decoration::Color::White);
	generator->set(3, 1, Decoration::Triangle | 0x10000 | Decoration::Color::White);
	generator->set(5, 3, Decoration::Star | Decoration::Color::Green);
	generator->write(0x03612);
}

void PuzzleList::GenerateBunkerH()
{
	//I would randomize this, if I could get the panels to actually render the symbols.
	//Unfortunately, the path is rendered to a 3D model that doesn't have any geometry between the grid lines.
	//Somehow, I would either have to change the model, or make the puzzle render to the background texture instead.
}

void PuzzleList::GenerateSwampH()
{
	generator->setLoadingData(L"Swamp", 55);
	generator->resetConfig();
	generator->setGridSize(3, 3);
	generator->setFlagOnce(Generate::Config::SplitShapes);
	generator->generate(0x0056E, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Arrow, 1);
	//First row
	generator->setGridSize(4, 4);
	generator->generate(0x00469, Decoration::Poly, 2, Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Arrow, 2, Decoration::Stone | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::White, 1);
	generator->generate(0x00472, Decoration::Poly, 2, Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Arrow, 2, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2);
	generator->generate(0x00262, Decoration::Poly, 2, Decoration::Triangle | Decoration::Color::Orange, 3, Decoration::Arrow, 2, Decoration::Stone | Decoration::Color::Black, 3, Decoration::Stone | Decoration::Color::White, 2);
	generator->generate(0x00474, Decoration::Poly, 3, Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Arrow, 3, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2);
	generator->generate(0x00553, Decoration::Poly, 3, Decoration::Triangle | Decoration::Color::Orange, 3, Decoration::Arrow, 3, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 3);
	generator->generate(0x0056F, Decoration::Poly, 3, Decoration::Triangle | Decoration::Color::Orange, 4, Decoration::Arrow, 3, Decoration::Stone | Decoration::Color::Black, 3, Decoration::Stone | Decoration::Color::White, 3);
	//Second Row
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(0x00390, Decoration::Poly, 2, Decoration::Arrow, 3);
	generator->generate(0x010CA, Decoration::Poly, 3, Decoration::Arrow, 3);
	generator->generate(0x00983, Decoration::Poly, 3, Decoration::Arrow, 4);
	generator->generate(0x00984, Decoration::Poly, 4, Decoration::Arrow, 4);
	generator->setGridSize(5, 5);
	generator->generate(0x00986, Decoration::Poly, 2, Decoration::Triangle | Decoration::Color::Orange, 4, Decoration::Arrow, 2);
	generator->generate(0x00985, Decoration::Poly, 3, Decoration::Triangle | Decoration::Color::Orange, 3, Decoration::Arrow, 4);
	generator->generate(0x00987, Decoration::Poly, 4, Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Arrow, 3);
	generator->setGridSize(6, 6);
	generator->pathWidth = 0.6f;
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x181A9, Decoration::Poly, 3, Decoration::Triangle | Decoration::Color::Orange, 3, Decoration::Arrow, 3, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2);
	//Modify bridge
	generator->resetConfig();
	generator->initPanel(0x00609);
	generator->set(5, 1, Decoration::Stone | Decoration::Color::White);
	generator->set(11, 1, Decoration::Stone | Decoration::Color::White);
	generator->set(3, 5, Decoration::Stone | Decoration::Color::Black);
	generator->set(9, 5, Decoration::Stone | Decoration::Color::Black);
	generator->setFlag(Generate::Config::DecorationsOnly);
	generator->setFlag(Generate::Config::DisableReset);
	generator->write(0x00609);
	generator->write(0x18488);
	generator->resetConfig();
	//Turn off bridge control until all previous puzzles are solved
	specialCase->setTargetAndDeactivate(0x181A9, 0x00609);
	//Red Panels
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->arrowColor = { 1, 1, 0, 1 };
	generator->setSymbol(Decoration::Start, 4, 4);
	generator->generate(0x00982, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Arrow, 2);
	generator->setSymbol(Decoration::Start, 4, 4);
	generator->generate(0x0097F, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Arrow, 3);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->setSymbol(Decoration::Start, 6, 4);
	generator->generate(0x0098F, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Arrow, 4);
	generator->setSymbol(Decoration::Start, 6, 4);
	generator->generate(0x00990, Decoration::Poly | Decoration::Can_Rotate, 4, Decoration::Arrow, 4);
	generator->setFlagOnce(Generate::Config::DisableReset);
	generator->setFlagOnce(Generate::Config::LongPath);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->setSymbol(Decoration::Start, 4, 6);
	generator->generate(0x17C0D, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Arrow, 3);
	generator->place_gaps(9);
	generator->write(0x17C0E);
	//Disconnected Shapes
	generator->resetConfig();
	generator->setFlag(Generate::Config::DisconnectShapes);
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->setSymbol(Decoration::Start, 4, 4);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x00999, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Arrow, 2);
	generator->setSymbol(Decoration::Start, 4, 4);
	generator->generate(0x0099D, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Arrow, 3);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->setSymbol(Decoration::Start, 4, 6);
	generator->generate(0x009A0, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Arrow, 2);
	generator->setSymbol(Decoration::Start, 6, 4);
	generator->generate(0x009A1, Decoration::Poly | Decoration::Can_Rotate, 4, Decoration::Arrow, 3);
	//Modify rotating bridge
	generator->initPanel(0x181F5);
	generator->set(1, 1, Decoration::Star | Decoration::Color::Black);
	generator->set(9, 3, Decoration::Triangle3 | Decoration::Color::Black);
	generator->set(9, 7, Decoration::Triangle3 | Decoration::Color::Black);
	generator->set(1, 9, Decoration::Stone | Decoration::Color::Black);
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	generator->write(0x181F5);
	//Full Dot Shapes
	generator->resetConfig();
	generator->arrowColor = { 1, 1, 0, 1 };
	generator->setFlag(Generate::Config::WriteDotColor);
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(0x00007, Decoration::Arrow, 2, Decoration::Dot_Intersection, 25, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x00008, Decoration::Arrow, 3, Decoration::Dot_Intersection, 25, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->generate(0x00009, Decoration::Arrow, 2, Decoration::Dot_Intersection, 36, Decoration::Poly, 1, Decoration::Poly | Decoration::Can_Rotate, 1, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x0000A, Decoration::Arrow, 3, Decoration::Dot_Intersection, 36, Decoration::Poly, 1, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setGridSize(6, 6);
	generator->pathWidth = 0.6f;
	generator->generate(0x003B2, Decoration::Arrow, 3, Decoration::Dot_Intersection, 49, Decoration::Poly, 3, Decoration::Start, 1);
	generator->generate(0x00A1E, Decoration::Arrow, 4, Decoration::Dot_Intersection, 49, Decoration::Poly, 1, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x00C2E, Decoration::Arrow, 3, Decoration::Dot_Intersection, 49, Decoration::Dot_Intersection, 49, Decoration::Poly, 2, Decoration::Poly | Decoration::Can_Rotate, 2, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(0x00E3A, Decoration::Arrow, 4, Decoration::Dot_Intersection, 49, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Start, 1, Decoration::Exit, 1);
	//Optional Tetris
	generator->resetConfig();
	generator->setGridSize(6, 6);
	generator->pathWidth = 0.6f;
	generator->arrowColor = { 0.5f, 1, 0, 1 };
	generator->setFlagOnce(Generate::Config::WriteDotColor);
	generator->generate(0x009A6, Decoration::Dot_Intersection, 49, Decoration::Poly, 1, Decoration::Poly | Decoration::Can_Rotate, 2,
		Decoration::Triangle | Decoration::Color::White, 3, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::White, 2, Decoration::Arrow, 3);
	//Negative Shapes 1
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->arrowColor = { 0, 0, 1, 1 };
	generator->generate(0x009AB, Decoration::Arrow, 1, Decoration::Poly, 2, Decoration::Poly | Decoration::Negative, 2);
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	generator->generate(0x009AD, Decoration::Arrow, 2, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 2);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->generate(0x009AE, Decoration::Arrow, 2, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 3);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x009AF, Decoration::Arrow, 3, Decoration::Poly, 4, Decoration::Poly | Decoration::Negative, 4);
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(0x00006, Decoration::Arrow, 3, Decoration::Poly, 4, Decoration::Poly | Decoration::Negative, 3);
	//Negative Shapes 2
	generator->arrowColor = {1, 1, 1, 1 };
	generator->setGridSize(6, 6);
	generator->generate(0x00002, Decoration::Dot_Intersection, 3, Decoration::Arrow, 2, Decoration::Star | Decoration::Color::Green, 2, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Magenta, 3, Decoration::Poly, 2, Decoration::Poly | Decoration::Negative, 1);
	generator->generate(0x00004, Decoration::Dot_Intersection, 3, Decoration::Arrow, 3, Decoration::Star | Decoration::Color::Green, 3, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Green, 3, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 2);
	generator->generate(0x00005, Decoration::Dot_Intersection, 4, Decoration::Arrow, 3, Decoration::Star | Decoration::Color::Green, 2, Decoration::Star | Decoration::Color::Magenta, 3, Decoration::Stone | Decoration::Color::Magenta, 3, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 3);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x013E6, Decoration::Dot_Intersection, 4, Decoration::Arrow, 3, Decoration::Star | Decoration::Color::Green, 3, Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Stone | Decoration::Color::Green, 3, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 3);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x00596, Decoration::Dot_Intersection, 5, Decoration::Arrow, 3, Decoration::Star | Decoration::Color::Green, 2, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Stone | Decoration::Color::Magenta, 3, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 2);
	//Negative Shapes 3
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteDotColor);
	generator->pathWidth = 0.6f;
	generator->setGridSize(6, 6);
	generator->setFlag(Generate::Config::BigShapes);
	generator->setFlag(Generate::Config::RequireCancelShapes);
	generator->generate(0x00001, Decoration::Eraser, 1, Decoration::Dot_Intersection, 49, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 3, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x014D2, Decoration::Eraser, 1, Decoration::Dot_Intersection, 49, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 3, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setGridSize(6, 6);
	generator->generate(0x014D4, Decoration::Eraser, 1, Decoration::Dot_Intersection, 49, Decoration::Poly, 4, Decoration::Poly | Decoration::Negative, 4, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x014D1, Decoration::Eraser, 1, Decoration::Dot_Intersection, 49, Decoration::Poly, 4, Decoration::Poly | Decoration::Negative, 4, Decoration::Start, 1, Decoration::Exit, 1);
	//Modify Swamp Island
	generator->resetConfig();
	generator->initPanel(0x17C0A);
	generator->set(5, 3, 0);
	generator->set(3, 1, Decoration::Poly | Decoration::Can_Rotate | Decoration::Negative | 0x00130000);
	generator->setFlag(Generate::Config::DecorationsOnly);
	generator->setFlag(Generate::Config::DisableReset);
	generator->write(0x17C0A);
	generator->write(0x17E07);
	//Exit Shortcut
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.7f;
	generator->generate(0x17C05, Decoration::Poly | Decoration::Orange, 3, Decoration::Poly | Decoration::Negative | Decoration::Orange, 3,
		Decoration::Star | Decoration::Color::Orange, 4);
	generator->generate(0x17C02, Decoration::Poly | Decoration::Blue, 3, Decoration::Poly | Decoration::Negative | Decoration::Blue, 3,
		Decoration::Star | Decoration::Color::Blue, 3);
}

void PuzzleList::GenerateTreehouseH()
{
	generator->setLoadingData(L"Treehouse", 58);
	generator->resetConfig();
	generator->arrowColor = { 0, 0, 1, 0 };
	generator->successColor = { 0, 0, 1, 0 };
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->setGridSize(2, 2);
	generator->generate(0x0288C, Decoration::Star | Decoration::Color::Orange, 1, Decoration::Triangle | Decoration::Color::Orange, 1, Decoration::Stone | Decoration::Color::Orange, 1);
	generator->setGridSize(3, 3);
	generator->generate(0x02886, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Triangle | Decoration::Color::Orange, 2);
	generator->setFlag(Generate::Config::TreehouseLayout);
	//Yellow Bridge
	generator->setGridSize(4, 4);
	generator->generate(0x17D72, Decoration::Star | Decoration::Color::Orange, 1, Decoration::Triangle | Decoration::Color::Orange, 7);
	generator->generate(0x17D8F, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Triangle | Decoration::Color::Orange, 6);
	generator->generate(0x17D74, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Triangle | Decoration::Color::Orange, 6);
	generator->generate(0x17DAC, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Triangle | Decoration::Color::Orange, 5);
	generator->generate(0x17D9E, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Triangle | Decoration::Color::Orange, 5);
	generator->generate(0x17DB9, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Triangle | Decoration::Color::Orange, 4);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.8f;
	generator->generate(0x17D9C, Decoration::Star | Decoration::Color::Orange, 5, Decoration::Triangle | Decoration::Color::Orange, 6);
	generator->generate(0x17DC2, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Triangle | Decoration::Color::Orange, 6);
	generator->generate(0x17DC4, Decoration::Star | Decoration::Color::Orange, 6, Decoration::Triangle | Decoration::Color::Orange, 5);
	generator->removeFlagOnce(Generate::Config::TreehouseLayout);
	generator->generate(0x0A182, Decoration::Star | Decoration::Color::Orange, 5, Decoration::Triangle | Decoration::Color::Orange, 3, Decoration::Stone | Decoration::Color::Orange, 3);
	//Pink Bridge 1
	generator->setGridSize(4, 4);
	generator->pathWidth = 1;
	generator->generate(0x17DC8, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Dot_Intersection, 25);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.8f;
	generator->generate(0x17DC7, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Dot_Intersection, 36);
	generator->generate(0x17CE4, Decoration::Star | Decoration::Color::Magenta, 6, Decoration::Dot_Intersection, 36);
	generator->generate(0x17D2D, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Dot_Intersection, 36);
	generator->generate(0x17D6C, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Orange, 6, Decoration::Dot_Intersection, 36);
	//Pink Bridge 2
	generator->setGridSize(4, 4);
	generator->pathWidth = 1;
	generator->removeFlag(Generate::Config::TreehouseColors);
	generator->setFlag(Generate::Config::WriteColors);
	generator->generate(0x17D9B, Decoration::Triangle | Decoration::Color::Magenta, 4,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2,
		Decoration::Star | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::White, 1);
	generator->generate(0x17D99, Decoration::Triangle | Decoration::Color::Magenta, 4,
		Decoration::Stone | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::White, 1,
		Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 2);
	generator->generate(0x17DAA, Decoration::Triangle | Decoration::Color::Magenta, 4,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2, Decoration::Star | Decoration::Color::Magenta, 2);
	generator->generate(0x17D97, Decoration::Triangle | Decoration::Color::Magenta, 4,
		Decoration::Stone | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::Black, 1,
		Decoration::Stone | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::White, 2);
	generator->removeFlag(Generate::Config::WriteColors);
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->generate(0x17BDF, Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Triangle | Decoration::Color::Magenta, 2,
		Decoration::Star | Decoration::Color::Magenta, 2, Decoration::Star | Decoration::Color::Green, 2,
		Decoration::Stone | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::Green, 2);
	generator->setGridSize(5, 4);
	generator->pathWidth = 0.8f;
	generator->generate(0x17D91, Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Triangle | Decoration::Color::Magenta, 2,
		Decoration::Star | Decoration::Color::Magenta, 3, Decoration::Star | Decoration::Color::Green, 4,
		Decoration::Stone | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::Green, 2);
	generator->generate(0x17DC6, Decoration::Triangle | Decoration::Color::Green, 3, Decoration::Triangle | Decoration::Color::Magenta, 2,
		Decoration::Star | Decoration::Color::Magenta, 3, Decoration::Star | Decoration::Color::Orange, 3,
		Decoration::Stone | Decoration::Color::Orange, 2, Decoration::Stone | Decoration::Color::Green, 2);
	//Orange Bridge 1	
	generator->removeFlag(Generate::Config::TreehouseColors);
	generator->setFlag(Generate::Config::WriteColors);
	generator->setGridSize(4, 4);
	generator->pathWidth = 1;
	generator->generate(0x17DB3, Decoration::Star | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::White, 2,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 1, Decoration::Poly | Decoration::Can_Rotate | Decoration::Orange, 1, Decoration::Poly | Decoration::Orange, 1);
	generator->generate(0x17DB5, Decoration::Star | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::White, 2,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 1, Decoration::Poly | Decoration::Can_Rotate | Decoration::Black, 1, Decoration::Poly | Decoration::Black, 1);
	generator->generate(0x17DB6, Decoration::Star | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::White, 2,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 1, Decoration::Poly | Decoration::Can_Rotate | Decoration::White, 1, Decoration::Poly | Decoration::White, 1);
	generator->generate(0x17DC0, Decoration::Star | Decoration::Color::Black, 1, Decoration::Star | Decoration::Color::White, 2,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 1,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::Black, 1, Decoration::Poly | Decoration::White, 1);
	generator->removeFlag(Generate::Config::WriteColors);
	generator->setFlag(Generate::Config::AlternateColors); //Black -> Green, White -> Pink, Purple -> White, Green -> Black
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.8f;
	generator->generate(0x17DD7, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 4,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::Black, 1, Decoration::Poly | Decoration::Black, 1);
	generator->generate(0x17DD9, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::White, 3,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2,
		Decoration::Poly | Decoration::Black, 1, Decoration::Poly | Decoration::Can_Rotate | Decoration::White, 1);
	generator->setFlag(Generate::Config::BigShapes);
	generator->generate(0x17DB8, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::White, 3,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::White, 1, Decoration::Poly | Decoration::White, 1);
	generator->generate(0x17DDC, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::White, 4,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::Black, 1, Decoration::Poly | Decoration::White, 1);
	generator->removeFlag(Generate::Config::BigShapes);
	generator->setObstructions({ { 1, 2 },{ 1, 4 },{ 9, 2 },{ 9, 4 },{ 2, 1 },{ 4, 1 },{ 6, 1 },{ 8, 1 } });
	generator->generate(0x17DD1, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::White, 3,
		Decoration::Stone | Decoration::Color::White, 2, Decoration::Stone | Decoration::Color::Purple, 2,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::Purple, 1, Decoration::Poly | Decoration::Black, 1);
	generator->generate(0x17DDE, Decoration::Star | Decoration::Color::Purple, 2, Decoration::Star | Decoration::Color::Black, 5,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::Purple, 1,
		Decoration::Poly | Decoration::Purple, 1, Decoration::Poly | Decoration::Can_Rotate | Decoration::White, 1);
	generator->setFlag(Generate::Config::BigShapes);
	generator->generate(0x17DE3, Decoration::Star | Decoration::Color::Purple, 4, Decoration::Star | Decoration::Color::White, 4,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::Purple, 1,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::Purple, 1, Decoration::Poly | Decoration::Black, 1);
	generator->removeFlag(Generate::Config::BigShapes);
	generator->removeFlag(Generate::Config::AlternateColors);
	generator->generate(0x17DEC, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 3,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 1, Decoration::Triangle | Decoration::Color::Black, 2, Decoration::Triangle | Decoration::Color::White, 1,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::Black, 1, Decoration::Poly | Decoration::White, 1);
	specialCase->clearTarget(0x17DEC);
	generator->generate(0x17DAE, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::White, 3,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 1, Decoration::Triangle | Decoration::Color::Black, 1, Decoration::Triangle | Decoration::Color::White, 2,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::Black, 1, Decoration::Poly | Decoration::White, 1);
	generator->generate(0x17DB0, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::White, 2,
		Decoration::Stone | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::White, 2, Decoration::Triangle | Decoration::Color::Black, 1, Decoration::Triangle | Decoration::Color::White, 1,
		Decoration::Poly | Decoration::Black, 1, Decoration::Poly | Decoration::Can_Rotate | Decoration::White, 1);
	generator->generate(0x17DDB, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::White, 3,
		Decoration::Stone | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::White, 1,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::Black, 1, Decoration::Poly | Decoration::Can_Rotate | Decoration::White, 1,
		Decoration::Triangle | Decoration::Black, 1, Decoration::Triangle | Decoration::White, 1);
	(new TreehouseWatchdog(0x17DAE))->start();
	//Orange Bridge 2
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->pathWidth = 1;
	generator->setGridSize(3, 3);
	generator->generate(0x17D88, Decoration::Star | Decoration::Color::Orange, 2, Decoration::Star | Decoration::Color::Magenta, 1,
		Decoration::Triangle | Decoration::Color::Orange, 1, Decoration::Triangle | Decoration::Color::Magenta, 1);
	generator->setGridSize(4, 4);
	generator->generate(0x17DB4, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::Magenta, 2,
		Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Triangle | Decoration::Color::Magenta, 2);
	generator->generate(0x17D8C, Decoration::Star | Decoration::Color::Orange, 1, Decoration::Star | Decoration::Color::Magenta, 2,
		Decoration::Triangle | Decoration::Color::Orange, 3, Decoration::Triangle | Decoration::Color::Magenta, 2);
	specialCase->generatePivotPanel(0x17CE3, { 4, 4 }, { { Decoration::Triangle | Decoration::Color::Orange, 3 },{ Decoration::Triangle | Decoration::Color::Magenta, 2 } }, generator->colorblind);
	generator->setGridSize(5, 4);
	generator->pathWidth = 0.8f;
	generator->generate(0x17DCD, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::Magenta, 3,
		Decoration::Triangle | Decoration::Color::Orange, 3, Decoration::Triangle | Decoration::Color::Magenta, 3);
	generator->generate(0x17DB2, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::Magenta, 4,
		Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Triangle | Decoration::Color::Magenta, 2);
	generator->generate(0x17DCC, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::Magenta, 3, Decoration::Star | Decoration::Color::Green, 6,
		Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Triangle | Decoration::Color::Magenta, 2);
	generator->generate(0x17DCA, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Green, 6,
		Decoration::Triangle | Decoration::Color::Orange, 1, Decoration::Triangle | Decoration::Color::Magenta, 1);
	generator->generate(0x17D8E, Decoration::Star | Decoration::Color::Orange, 3, Decoration::Star | Decoration::Color::Magenta, 3, Decoration::Star | Decoration::Color::Green, 2,
		Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Triangle | Decoration::Color::Magenta, 2, Decoration::Triangle | Decoration::Color::Green, 2);
	specialCase->generatePivotPanel(0x17DB7, { 4, 4 }, { { Decoration::Triangle | Decoration::Color::Orange, 2 },{ Decoration::Triangle | Decoration::Color::Magenta, 2 },{ Decoration::Triangle | Decoration::Color::Green, 1 } }, generator->colorblind);
	generator->generate(0x17DB1, { { Decoration::Star | Decoration::Color::Orange, 3 },{ Decoration::Star | Decoration::Color::Magenta, 3 },{ Decoration::Star | Decoration::Color::White, 3 },
		{ Decoration::Star | Decoration::Color::Green, 3 },
		{ Decoration::Triangle | Decoration::Color::Orange, 1 },{ Decoration::Triangle | Decoration::Color::Magenta, 1 },{ Decoration::Triangle | Decoration::Color::White, 1 },
		{ Decoration::Triangle | Decoration::Color::Green, 1 } });
	generator->generate(0x17DA2, { { Decoration::Star | Decoration::Color::Orange, 2 },{ Decoration::Star | Decoration::Color::Magenta, 2 },{ Decoration::Star | Decoration::Color::White, 2 },
		{ Decoration::Star | Decoration::Color::Green, 2 },{ Decoration::Star | Decoration::Color::Black, 2 },
		{ Decoration::Triangle | Decoration::Color::Orange, 1 },{ Decoration::Triangle | Decoration::Color::Magenta, 1 },{ Decoration::Triangle | Decoration::Color::White, 1 },
		{ Decoration::Triangle | Decoration::Color::Green, 1 },{ Decoration::Triangle | Decoration::Color::Black, 1 } });
	//Green Bridge
	generator->removeFlag(Generate::Config::TreehouseColors);
	generator->setFlag(Generate::Config::AlternateColors);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.7f;
	generator->generate(0x17E3C, Decoration::Poly | Decoration::Color::White, 3, Decoration::Poly | Decoration::Negative | Decoration::Color::Green, 1,
		Decoration::Star | Decoration::Color::White, 5, Decoration::Star | Decoration::Color::Green, 3);
	generator->generate(0x17E4D, Decoration::Poly | Decoration::Color::Green, 3, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 2,
		Decoration::Star | Decoration::Color::White, 5, Decoration::Star | Decoration::Color::Green, 3);
	generator->generate(0x17E4F, Decoration::Poly | Decoration::Color::White, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::Green, 1,
		Decoration::Poly | Decoration::Color::Green, 1, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 1,
		Decoration::Star | Decoration::Color::White, 5, Decoration::Star | Decoration::Color::Green, 4);
	generator->setObstructions({ { { 1, 2 },{ 1, 4 },{ 9, 2 },{ 9, 4 },{ 2, 1 },{ 4, 1 },{ 6, 1 },{ 8, 1 } },
		{ { 1, 2 },{ 1, 4 },{ 1, 6 },{ 0, 7 },{ 9, 2 },{ 9, 4 },{ 9, 6 },{ 10, 7 },{ 4, 1 },{ 6, 1 },{ 8, 1 } },
		{ { 1, 2 },{ 1, 4 },{ 1, 6 },{ 0, 7 },{ 9, 2 },{ 9, 4 },{ 9, 6 },{ 10, 7 },{ 2, 1 },{ 4, 1 },{ 6, 1 } } });
	generator->generate(0x17E52, Decoration::Poly | Decoration::Color::Green, 1, Decoration::Poly | Decoration::Color::Black, 1, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 3,
		Decoration::Star | Decoration::Color::White, 3, Decoration::Star | Decoration::Color::Green, 2, Decoration::Star | Decoration::Color::Black, 3);
	generator->generate(0x17E5B, Decoration::Poly | Decoration::Color::White, 1, Decoration::Poly | Decoration::Color::Black, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::Green, 2,
		Decoration::Star | Decoration::Color::White, 3, Decoration::Star | Decoration::Color::Green, 3, Decoration::Star | Decoration::Color::Black, 3);
	generator->generate(0x17E5F, Decoration::Poly | Decoration::Color::Black, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::Green, 2,
		Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::Green, 3,
		Decoration::Triangle | Decoration::Color::Black, 2, Decoration::Triangle | Decoration::Color::Green, 2);
	generator->generate(0x17E61, { { Decoration::Poly | Decoration::Color::White, 1 },{ Decoration::Poly | Decoration::Color::Green, 1 },{ Decoration::Poly | Decoration::Color::Cyan, 1 },
		{ Decoration::Poly | Decoration::Negative | Decoration::Color::Green, 1 },{ Decoration::Poly | Decoration::Negative | Decoration::Color::Black, 2 },
		{ Decoration::Star | Decoration::Color::Cyan, 2 },{ Decoration::Star | Decoration::Color::Black, 1 },{ Decoration::Star | Decoration::Color::White, 1 },{ Decoration::Star | Decoration::Color::Green, 2 },
		{ Decoration::Triangle | Decoration::Color::Black, 2 },{ Decoration::Triangle | Decoration::Color::White, 1 } ,{ Decoration::Triangle | Decoration::Color::Cyan, 2 } });
}


void PuzzleList::GenerateTownH()
{
	generator->setLoadingData(L"Town", 21);
	generator->resetConfig();
	//Full Dots + Triangles
	generator->setFlag(Generate::Config::EnableFlash);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.9f;
	generator->generate(0x2899C, Decoration::Dot_Intersection, 36, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Orange, 3, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x28A33, Decoration::Stone | Decoration::Color::Cyan, 5, Decoration::Stone | Decoration::Color::Yellow, 5, Decoration::Dot_Intersection, 36, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Orange, 2, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x28ABF, Decoration::Star | Decoration::Color::Cyan, 6, Decoration::Star | Decoration::Color::Yellow, 6, Decoration::Dot_Intersection, 36, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Orange, 2, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x28AC0, Decoration::Poly | Decoration::Negative, 7, Decoration::Dot_Intersection, 36, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Orange, 2, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x28AC1, Decoration::Triangle, 5, Decoration::Dot_Intersection, 36, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Orange, 3, Decoration::Start, 1, Decoration::Exit, 1);
	generator->arrowColor = { 0, 1, 0.8f, 1 };
	generator->generate(0x28AD9, Decoration::Arrow, 5, Decoration::Triangle, 4, 
		Decoration::Dot_Intersection, 36, Decoration::Start, 1, Decoration::Exit, 1);
	//Blue Symmetry
	generator->resetConfig();
	generator->setFlag(Generate::Config::RequireCombineShapes);
	generator->setGridSize(7, 7);
	generator->pathWidth = 0.6f;
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x28AC7, Decoration::Triangle | Decoration::Color::Yellow, 9);
	generator->setSymmetry(Panel::Symmetry::RotateLeft);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x28AC8, Decoration::Stone | Decoration::Color::Black, 7, Decoration::Stone | Decoration::Color::White, 7);
	generator->setSymmetry(Panel::Symmetry::RotateRight);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x28ACA, Decoration::Star | Decoration::Color::Black, 8, Decoration::Star | Decoration::Color::White, 8);
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->setSymmetry(Panel::Symmetry::RotateLeft);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x28ACB, Decoration::Poly | Decoration::Color::Orange, 5);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setSymbol(Decoration::Start, 4, 4); generator->setSymbol(Decoration::Start, 10, 4); generator->setSymbol(Decoration::Start, 4, 10); generator->setSymbol(Decoration::Start, 10, 10);
	generator->setSymbol(Decoration::Exit, 4, 0); generator->setSymbol(Decoration::Exit, 14, 4); generator->setSymbol(Decoration::Exit, 0, 10); generator->setSymbol(Decoration::Exit, 10, 14);
	generator->generate(0x28ACC, Decoration::Eraser, 1, Decoration::Triangle | Decoration::Color::Yellow, 9);
	generator->resetConfig();
	//Glass Door
	generator->setFlagOnce(Generate::Config::SmallShapes);
	generator->setFlagOnce(Generate::Config::EnableFlash);
	generator->setFlagOnce(Generate::Config::WriteColors);
	generator->generate(0x28998, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Black, 3, Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::White, 2,
		Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::White, 3, Decoration::Triangle | Decoration::Color::White, 3, Decoration::Triangle | Decoration::Color::Black, 3);
	//Church Star Door
	specialCase->generateColorFilterPuzzle(0x28A0D, { 5, 5 }, { std::make_pair<int, int>(Decoration::Star | 1, 6), std::make_pair<int, int>(Decoration::Star | 2, 6),
		std::make_pair<int,int>(Decoration::Star | 3, 6), std::make_pair<int,int>(Decoration::Star | 4, 6) }, { 1, 1, 0, 0 });
	//Mess with targets
	specialCase->copyTarget(0x03C08, 0x28A0D); specialCase->copyTarget(0x28A0D, 0x28998);
	specialCase->setTargetAndDeactivate(0x28998, 0x28A0D); specialCase->setTargetAndDeactivate(0x03C0C, 0x03C08);
	specialCase->setPower(0x28A69, false); (new TownDoorWatchdog())->start();
	//Soundproof Room
	std::vector<int> allPitches = { DOT_SMALL, DOT_SMALL, DOT_MEDIUM, DOT_MEDIUM, DOT_LARGE, DOT_LARGE };
	std::vector<int> pitches;
	for (int i = 0; i < 4; i++) pitches.push_back(pop_random(allPitches));
	specialCase->generateSoundDotPuzzle(0x034E3, { 4, 4 }, pitches, false);
	generator->resetConfig();
	//3-color Room
	generator->initPanel(0x334D8);
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	generator->write(0x334D8);
	specialCase->generateRGBStonePuzzleH(0x03C0C);
	specialCase->generateRGBDotPuzzleH(0x03C08);
	//Orange Crate
	generator->setGridSize(7, 7);
	generator->setFlagOnce(Generate::Config::EnableFlash);
	generator->generate(0x0A0C8, Decoration::Star | Decoration::Black, 10, Decoration::Star | Decoration::Magenta, 10, Decoration::Star | Decoration::Orange, 8, Decoration::Star | Decoration::Yellow, 8);
	//Windmill Puzzles
	generator->resetConfig();
	generator->setGridSize(5, 5);
	generator->setFlag(Generate::Config::EnableFlash);
	generator->arrowColor = { 0, 1, 0, 1 };
	generator->generate(0x17F89, Decoration::Arrow, 3, Decoration::Stone | Decoration::Color::Orange, 4, Decoration::Stone | Decoration::Color::White, 4, Decoration::Triangle | Decoration::Color::Orange, 3);
	generator->generate(0x0A168, Decoration::Arrow, 3, Decoration::Stone | Decoration::Color::Orange, 5, Decoration::Star | Decoration::Color::Orange, 5, Decoration::Poly | Decoration::Color::Orange, 4);
	generator->generate(0x33AB2, Decoration::Arrow, 3, Decoration::Stone | Decoration::Color::White, 5, Decoration::Star | Decoration::Color::White, 5, Decoration::Poly | Decoration::Color::White, 4);
}

void PuzzleList::GenerateVaultsH()
{
	generator->setLoadingData(L"Vaults", 5);
	generator->resetConfig();
	//Tutorial Vault
	generator->setGridSize(8, 8);
	generator->setSymbol(Decoration::Exit, 0, 0);
	generator->setSymbol(Decoration::Exit, 16, 0);
	generator->setSymbol(Decoration::Exit, 0, 16);
	generator->setSymbol(Decoration::Exit, 16, 16);
	generator->generate(0x033D4, Decoration::Star | Decoration::Color::Black, 7, Decoration::Star | Decoration::Color::White, 7, 
		Decoration::Stone | Decoration::Color::Black, 7, Decoration::Stone | Decoration::Color::White, 7, Decoration::Dot_Intersection, 81, Decoration::Start, 5);
	//Desert Vault
	generator->resetConfig();
	generator->setFlag(Generate::Config::RequireCancelShapes);
	generator->setGridSize(7, 7);
	generator->generate(0x0CC7B, Decoration::Start, 1, Decoration::Arrow, 6, Decoration::Dot_Intersection, 64, 
		Decoration::Triangle | Decoration::Color::Orange, 4, Decoration::Triangle | Decoration::Color::Magenta, 3,  
		Decoration::Poly | Decoration::Color::Orange, 3, Decoration::Poly | Decoration::Color::Magenta, 2, Decoration::Poly | Decoration::Negative, 4);
	//Symmetry Vault
	generator->resetConfig();
	generator->setGridSize(8, 8);
	generator->setSymmetry(Panel::Symmetry::RotateRight);
	generator->setSymbol(Decoration::Start, 0, 16); generator->setSymbol(Decoration::Start, 16, 0);
	generator->setSymbol(Decoration::Start, 0, 0); generator->setSymbol(Decoration::Start, 16, 16);
	generator->setSymbol(Decoration::Exit, 8, 0); generator->setSymbol(Decoration::Exit, 8, 16);
	generator->setSymbol(Decoration::Exit, 0, 8); generator->setSymbol(Decoration::Exit, 16, 8);
	generator->generate(0x002A6, Decoration::Triangle | Decoration::Color::Cyan, 3, Decoration::Triangle | Decoration::Color::Yellow, 2,
		Decoration::Star | Decoration::Color::Cyan, 3, Decoration::Star | Decoration::Color::Yellow, 3,
		Decoration::Stone | Decoration::Color::Cyan, 2, Decoration::Stone | Decoration::Color::Yellow, 3);
	//Shipwreck Vault
	generator->resetConfig();
	specialCase->generateSoundDotReflectionPuzzle(0x00AFB, { 6, 6 }, { DOT_MEDIUM, DOT_LARGE, DOT_MEDIUM, DOT_SMALL }, { DOT_MEDIUM, DOT_LARGE, DOT_MEDIUM, DOT_SMALL }, 2, true);
	//Jungle Vault
	generator->resetConfig();
	specialCase->generateJungleVault(0x15ADD);
}

void PuzzleList::GenerateTrianglePanelsH()
{
	generator->setLoadingData(L"Arrows", 14);
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->arrowColor = { 1, 1, 1, 1 };
	generator->generate(0x17CFB, Decoration::Arrow, 3);
	generator->generate(0x3C12B, Decoration::Arrow, 3);
	generator->generate(0x17CE7, Decoration::Arrow, 3);
	generator->generate(0x17CF0, Decoration::Arrow, 3);
	generator->generate(0x17C71, Decoration::Arrow, 3);
	generator->generate(0x17CF7, Decoration::Arrow, 4);
	generator->generate(0x17D01, Decoration::Arrow, 4);
	generator->generate(0x17F9B, Decoration::Arrow, 4);
	generator->generate(0x17D27, Decoration::Arrow, 4);
	generator->generate(0x17D28, Decoration::Arrow, 4);
	generator->generate(0x17FA9, Decoration::Arrow, 5);
	generator->generate(0x17FA0, Decoration::Arrow, 5);
	generator->generate(0x17C42, Decoration::Arrow, 5);
	generator->generate(0x17F93, Decoration::Arrow, 5);
	generator->resetConfig();
}

void PuzzleList::GenerateMountainH()
{
	std::wstring text = L"Mountain Perspective";
	SetWindowText(_handle, text.c_str());
	specialCase->generateMountaintop(0x17C34, {
		{ Decoration::Triangle | Decoration::Color::White, 2 },{ Decoration::Triangle | Decoration::Color::Black, 1 },
		{ Decoration::Star | Decoration::Color::White, 1 },{ Decoration::Star | Decoration::Color::Black, 1 } });

	generator->setLoadingData(L"Mountain", 39);
	generator->resetConfig();
	//Purple Bridge
	generator->setFlagOnce(Generate::Config::PreserveStructure);
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	std::set<Point> bpoints1 = { { 6, 3 },{ 5, 4 },{ 7, 4 } };
	std::set<Point> bpoints2 = { { 6, 5 },{ 5, 6 },{ 6, 7 },{ 7, 6 } };
	std::set<Point> bpoints3 = { { 3, 6 },{ 4, 7 } };
	if (Random::rand() % 2 == 0) generator->hitPoints = { generator->pick_random(bpoints1), generator->pick_random(bpoints2), generator->pick_random(bpoints3) };
	else generator->hitPoints = { generator->pick_random(bpoints3), generator->pick_random(bpoints2), generator->pick_random(bpoints1) };
	generator->setObstructions({ { 4, 1 },{ 6, 1 },{ 8, 1 } });
	generator->blockPos = { { 3, 1 },{ 5, 1 },{ 7, 1 },{ 9, 1 },{ 1, 1 },{ 11, 1 },{ 1, 11 },{ 11, 11 } };
	generator->setSymbol(Decoration::Gap_Row, 3, 4);
	generator->generate(0x09E39, Decoration::Star | Decoration::Color::Purple, 5, Decoration::Triangle | Decoration::Color::Purple, 6, Decoration::Eraser | Decoration::Color::Purple, 1);
	//Orange Row
	generator->resetConfig();
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.9f;
	generator->setFlag(Generate::Config::ResetColors);
	generator->generate(0x09E73, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Arrow, 4, Decoration::Triangle, 4, Decoration::Dot_Intersection, 36);
	generator->generate(0x09E75, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Arrow, 4, Decoration::Triangle, 4, Decoration::Stone | Decoration::Color::Black, 4, Decoration::Stone | Decoration::Color::White, 6);
	generator->generate(0x09E78, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Arrow, 4, Decoration::Triangle, 4, Decoration::Star | Decoration::Color::Black, 6, Decoration::Star | Decoration::Color::White, 6);
	generator->generate(0x09E79, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Arrow, 4, Decoration::Triangle, 4, 
		Decoration::Stone | Decoration::Color::Black, 4, Decoration::Stone | Decoration::Color::White, 4, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::White, 3);
	generator->generate(0x09E6C, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Arrow, 4, Decoration::Triangle, 3, Decoration::Poly | Decoration::Color::Green, 3,
		Decoration::Star | Decoration::Color::Green, 3,
		Decoration::Eraser | Decoration::Color::Green, 1);
	generator->setGridSize(6, 6);
	generator->pathWidth = 0.8f;
	generator->generate(0x09E6F, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot_Intersection, 49, Decoration::Triangle | Decoration::Color::Purple, 9);
	generator->setGridSize(7, 7);
	generator->generate(0x09E6B, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Dot_Intersection, 64, Decoration::Triangle | Decoration::Color::Purple, 13);
	//Green Row
	generator->resetConfig();
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.9f;
	generator->generate(0x09E7A, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Arrow, 7);
	generator->generate(0x09E71, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Arrow, 4, 
		Decoration::Stone | Decoration::Color::White, 4, Decoration::Star | Decoration::Color::White, 3, Decoration::Triangle | Decoration::Color::White, 3);
	generator->generate(0x09E72, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Arrow, 4, 
		Decoration::Stone | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::Black, 3, Decoration::Triangle | Decoration::Color::Black, 3);
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	generator->generate(0x09E69, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Arrow, 4,
								 Decoration::Poly | Decoration::Color::Orange, 3, Decoration::Poly | Decoration::Negative | Decoration::Color::Orange, 6, Decoration::Star | Decoration::Color::Orange, 4);
	generator->generate(0x09E7B, Decoration::Start, 1, Decoration::Exit, 1, Decoration::Arrow, 5, Decoration::Triangle | Decoration::Color::Green, 5, Decoration::Dot_Intersection, 36);
	//Purple Panels
	generator->setFlag(Generate::Config::DisconnectShapes);
	generator->generate(0x09EAD, Decoration::Arrow, 3, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Orange, 4);
	generator->generate(0x09EAF, Decoration::Arrow, 3, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Star | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Orange, 4);
	//Blue Row
	generator->resetConfig();
	generator->setSymmetry(Panel::Symmetry::RotateRight);
	generator->setObstructions({ { 4, 3 },{ 5, 4 },{ 5, 6 },{ 5, 8 },{ 5, 10 },{ 6, 9 },{ 7, 10 } });
	specialCase->initRotateGrid(generator);
	generator->generate(0x33AF5, Decoration::Arrow, 7);
	if (Random::rand() % 2 == 0) generator->setObstructions({ { 5, 4 },{ 5, 6 },{ 5, 8 },{ 5, 10 },{ 9, 4 },{ 9, 6 },{ 9, 8 },{ 9, 10 },{ 7, 0 },{ 7, 2 } });
	else generator->setObstructions({ { 3, 4 },{ 3, 6 },{ 3, 8 },{ 3, 10 },{ 7, 4 },{ 7, 6 },{ 7, 8 },{ 7, 10 },{ 5, 0 },{ 5, 2 },{ 9, 0 },{ 9, 2 } });
	specialCase->initRotateGrid(generator);
	generator->generate(0x33AF7, Decoration::Arrow, 6);
	generator->setObstructions({ { 0, 5 },{ 3, 0 },{ 3, 2 },{ 3, 10 },{ 4, 1 },{ 4, 3 },{ 4, 9 },{ 5, 0 },{ 5, 2 },{ 5, 4 },{ 5, 6 },{ 5, 8 },{ 5, 10 },
		{ 6, 1 },{ 6, 3 },{ 6, 5 },{ 6, 7 },{ 6, 9 },{ 7, 0 },{ 7, 2 },{ 7, 4 },{ 7, 6 },{ 7, 8 },{ 7, 10 },{ 8, 1 },{ 8, 3 },{ 8, 5 },{ 8, 7 },{ 8, 9 },
		{ 9, 0 },{ 9, 2 },{ 9, 4 },{ 9, 6 },{ 9, 8 },{ 9, 10 },{ 10, 1 },{ 10, 3 },{ 10, 5 },{ 10, 7 },{ 10, 9 } });
	generator->blockPos = { { 5, 9 },{ 7, 7 } };
	specialCase->initRotateGrid(generator);
	generator->setFlagOnce(Generate::Config::ShortPath);
	generator->generate(0x09F6E, Decoration::Arrow, 5);

	//Rainbow Row
	generator->resetConfig();
	generator->setGridSize(6, 6);
	generator->setFlag(Generate::Config::WriteColors);//CMY, stone star tetris triangle eraser dot 
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(0x09FD3, Decoration::Eraser | Decoration::Color::Magenta, 1, Decoration::Stone| Decoration::Color::Magenta, 4, Decoration::Star| Decoration::Color::Magenta, 4,
								 Decoration::Triangle | Decoration::Color::Magenta, 5, Decoration::Dot, 5, Decoration::Poly | Decoration::Color::Magenta, 2,
								 Decoration::Start, 1, Decoration::Exit, 1);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(0x09FD4, Decoration::Eraser | Decoration::Color::Yellow, 1, Decoration::Stone | Decoration::Color::Yellow, 4, Decoration::Star | Decoration::Color::Yellow, 4,
		Decoration::Triangle | Decoration::Color::Yellow, 5,  Decoration::Dot, 5, Decoration::Poly | Decoration::Color::Yellow, 2,
		Decoration::Start, 1, Decoration::Exit, 1);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(0x09FD6, Decoration::Eraser | Decoration::Color::Cyan, 1, Decoration::Stone | Decoration::Color::Cyan, 4, Decoration::Star | Decoration::Color::Cyan, 4,
		Decoration::Triangle | Decoration::Color::Cyan, 5, Decoration::Dot, 5, Decoration::Poly | Decoration::Color::Cyan, 2,
		Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x09FD7, Decoration::Eraser | Decoration::Color::Yellow, 1, Decoration::Eraser | Decoration::Color::Cyan, 1, 
		Decoration::Stone | Decoration::Color::Magenta, 4, Decoration::Star | Decoration::Color::Yellow, 4, 
		Decoration::Triangle | Decoration::Color::Cyan, 5, Decoration::Dot, 5, Decoration::Poly | Decoration::Color::Magenta, 2,
		Decoration::Start, 1, Decoration::Exit, 1);
	generator->setSymmetry(Panel::Symmetry::None);
	generator->setGridSize(5, 5);
	generator->generate(0x09FD8, Decoration::Arrow, 9);
	generator->resetConfig();

	specialCase->generateMultiPuzzle({ 0x09FCC, 0x09FCE, 0x09FCF, 0x09FD0, 0x09FD1, 0x09FD2 }, {
	{ { Decoration::Triangle | Decoration::Color::Green, 2 }, { Decoration::Star | Decoration::Color::Green, 2 } },
	{ { Decoration::Triangle | Decoration::Color::Orange, 3 }, { Decoration::Star | Decoration::Color::Orange, 2 } },
	{ { Decoration::Triangle | Decoration::Color::Cyan, 4 }, { Decoration::Star | Decoration::Color::Cyan, 2 } },
	{ { Decoration::Triangle | Decoration::Color::Magenta, 5 }, { Decoration::Star | Decoration::Color::Magenta, 4 } },
	{ { Decoration::Triangle | Decoration::Color::Yellow, 6 }, { Decoration::Star | Decoration::Color::Yellow, 4 } },
	{ { Decoration::Triangle | Decoration::Color::White, 7 }, { Decoration::Star | Decoration::Color::White, 4 } } }, true);

	specialCase->generate2BridgeH(0x09E86, 0x09ED8);
	(new BridgeWatchdog(0x09E86, 0x09ED8))->start();

	generator->resetConfig();
	specialCase->generateMountainFloorH();

	//Pillar Puzzles
	generator->resetConfig();
	generator->setGridSize(6, 5);
	generator->setFlag(Generate::Config::ResetColors);
	generator->generate(0x0383D, Decoration::Arrow, 2, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 3,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2);
	generator->removeFlagOnce(Generate::Config::ResetColors);
	generator->setFlagOnce(Generate::Config::TreehouseColors);
	generator->generate(0x0383A, Decoration::Arrow, 2, Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::Magenta, 2,
		Decoration::Star | Decoration::Color::Green, 4, Decoration::Star | Decoration::Color::White, 2,
		Decoration::Triangle | Decoration::Color::White, 1);
	generator->setGridSize(6, 5);
	specialCase->initPillarSymmetry(generator, 0x0383F, Panel::Symmetry::PillarParallel);
	generator->generate(0x0383F, Decoration::Arrow, 2, Decoration::Triangle | Decoration::Color::Orange, 8);
	specialCase->initPillarSymmetry(generator, 0x09E56, Panel::Symmetry::PillarHorizontal);
	generator->generate(0x09E56, Decoration::Arrow, 2, Decoration::Triangle | Decoration::Color::Orange, 4, Decoration::Dot_Intersection, 45);
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->setGridSize(6, 4);
	specialCase->initPillarSymmetry(generator, 0x03859, Panel::Symmetry::PillarRotational);
	generator->generate(0x03859, Decoration::Arrow, 2, Decoration::Poly, 3, Decoration::Stone | Decoration::Color::Black, 2,
		Decoration::Stone | Decoration::Color::White, 2);
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	generator->setFlagOnce(Generate::Config::DisableDotIntersection);
	generator->setGridSize(6, 5);
	specialCase->initPillarSymmetry(generator, 0x09E5A, Panel::Symmetry::PillarVertical);
	generator->generate(0x09E5A, Decoration::Arrow, 2,Decoration::Poly | Decoration::Color::Orange, 1, Decoration::Poly | Decoration::Negative | Decoration::Color::Blue, 2,
		Decoration::Poly | Decoration::Color::Blue, 1, Decoration::Poly | Decoration::Negative | Decoration::Color::Orange, 2,
		Decoration::Star | Decoration::Color::Orange, 1, Decoration::Star | Decoration::Color::Blue, 1, Decoration::Dot, 4, Decoration::Start, 3);
	generator->setFlag(Generate::Config::Write2Color);
	generator->setFlag(Generate::Config::WriteInvisible);
	specialCase->initPillarSymmetry(generator, 0x339BB, Panel::Symmetry::PillarHorizontal);
	generator->generate(0x339BB, Decoration::Arrow, 2, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 1,
		Decoration::Stone | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::White, 1,
		Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Dot_Intersection | Decoration::Color::Cyan, 2, Decoration::Dot_Intersection | Decoration::Color::Yellow, 2);
	specialCase->initPillarSymmetry(generator, 0x33961, Panel::Symmetry::PillarParallel);
	generator->generate(0x33961, Decoration::Arrow, 2, Decoration::Triangle | Decoration::Color::Orange, 2, Decoration::Triangle | Decoration::Color::Magenta, 1,
		Decoration::Star | Decoration::Color::Orange, 1, Decoration::Star | Decoration::Color::Magenta, 1, Decoration::Poly | Decoration::Color::Orange, 1,
		Decoration::Poly | Decoration::Negative | Decoration::Color::Magenta, 2, Decoration::Eraser | Decoration::Color::Magenta, 1);
}

void PuzzleList::GenerateCavesH()
{
	generator->setLoadingData(L"Caves", 51);
	generator->resetConfig();

	specialCase->createArrowSecretDoor(0x17FA2);

	//Arrow Puzzles
	generator->setGridSize(5, 5);
	generator->backgroundColor = { 0, 0, 0.5f, 1 };
	generator->arrowColor = { 0, 1, 0.8f, 1 };
	generator->successColor = { 0, 1, 0.8f, 1 };
	generator->generate(0x00FF8, Decoration::Stone | Decoration::Color::Black, 5, Decoration::Stone | Decoration::Color::White, 4, Decoration::Arrow, 6);
	
	generator->backgroundColor = { 0, 0, 0.1f, 1 };
	generator->arrowColor = { 0.6f, 0, 1, 1 };
	generator->successColor = { 0.6f, 0, 1, 1 };
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->generate(0x01A0D, Decoration::Arrow, 6);
	generator->setGridSize(6, 6);
	generator->pathWidth = 0.7f;
	generator->generate(0x018A0, Decoration::Arrow, 8);
	generator->pathWidth = 1;
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->generate(0x009A4, Decoration::Arrow, 6, Decoration::Star | Decoration::Color::Cyan, 6, Decoration::Star | Decoration::Color::Magenta, 6);
	generator->generate(0x00A72, Decoration::Arrow, 4, Decoration::Poly, 3, Decoration::Poly | Decoration::Negative, 1);

	generator->setGridSize(4, 4);
	generator->pathWidth = 1.0f;
	generator->generate(0x00190, Decoration::Arrow, 8);
	generator->generate(0x00558, Decoration::Arrow, 8);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->generate(0x00567, Decoration::Arrow, 12);
	generator->generate(0x006FE, Decoration::Arrow, 12);

	generator->setGridSize(4, 4);
	generator->pathWidth = 1.0f;
	generator->generate(0x008B8, Decoration::Arrow, 4, Decoration::Triangle | Decoration::Color::Orange, 4);
	generator->generate(0x00973, Decoration::Arrow, 5, Decoration::Triangle | Decoration::Color::Orange, 3);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->generate(0x0097B, Decoration::Arrow, 4, Decoration::Triangle | Decoration::Color::Orange, 8);
	generator->generate(0x0097D, Decoration::Arrow, 6, Decoration::Triangle | Decoration::Color::Orange, 6);
	generator->generate(0x0097E, Decoration::Arrow, 8, Decoration::Triangle | Decoration::Color::Orange, 4);
	generator->setGridSize(4, 4);
	generator->pathWidth = 1.0f;
	generator->generate(0x00994, Decoration::Arrow, 2, Decoration::Poly, 1, Decoration::Poly | Decoration::Can_Rotate, 1);
	generator->generate(0x334D5, Decoration::Arrow, 2, Decoration::Poly | Decoration::Can_Rotate, 2);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->generate(0x00995, Decoration::Arrow, 6, Decoration::Poly, 1, Decoration::Poly | Decoration::Can_Rotate, 1);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x00996, Decoration::Arrow, 4, Decoration::Poly, 1, Decoration::Poly | Decoration::Can_Rotate, 1);
	generator->generate(0x00998, Decoration::Arrow, 4, Decoration::Poly, 3);

	//First alcove
	generator->resetConfig();
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(0x32962, Decoration::Dot_Intersection, 49, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Start, 1);
	generator->setFlagOnce(Generate::Config::ResetColors);
	generator->setFlagOnce(Generate::Config::TreehouseLayout);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(0x32966, Decoration::Star | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::White, 2,
		Decoration::Stone | Decoration::Color::Black, 1, Decoration::Stone | Decoration::Color::White, 1, Decoration::Triangle | Decoration::Color::Black, 1, Decoration::Triangle | Decoration::Color::White, 1,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::Black, 1, Decoration::Poly | Decoration::Can_Rotate | Decoration::White, 1);
	generator->setGridSize(6, 6);
	generator->pathWidth = 1.0f;
	generator->setFlagOnce(Generate::Config::WriteColors);
	generator->generate(0x01A31, Decoration::Star | Decoration::Color::Cyan, 8, Decoration::Star | Decoration::Color::Yellow, 6,
		Decoration::Star | Decoration::Color::Magenta, 6);
	generator->setGridSize(6, 6);
	generator->pathWidth = 0.85f;
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	generator->setFlagOnce(Generate::Config::WriteDotColor);
	generator->generate(0x00B71, Decoration::Stone | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::White, 3,
		Decoration::Poly | Decoration::Color::White, 3, Decoration::Poly | Decoration::Negative | Decoration::Color::White, 2,
		Decoration::Dot, 49, Decoration::Eraser | Decoration::Color::Black, 1, Decoration::Start, 1, Decoration::Exit, 1);

	//Perspective
	generator->resetConfig();
	generator->setFlag(Generate::Config::EnableFlash);
	generator->setFlag(Generate::Config::DecorationsOnly);
	specialCase->generateCenterPerspective(0x288EA, { { Decoration::Star | Decoration::Color::Black, 8 },
		{ Decoration::Star | Decoration::Color::White, 6 } }, Decoration::Star);
	specialCase->generateCenterPerspective(0x288FC, { { Decoration::Poly, 4 },
		{ Decoration::Eraser | Decoration::Color::White, 1 } }, Decoration::Eraser);
	specialCase->generateCenterPerspective(0x289E7, { { Decoration::Triangle | Decoration::Color::Orange, 10},
		{ Decoration::Eraser | Decoration::Color::White, 1 } }, Decoration::Triangle);
	generator->removeFlag(Generate::Config::DecorationsOnly);
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	specialCase->generateCenterPerspective(0x288AA, { { Decoration::Dot_Intersection, 49 },{ Decoration::Poly, 2 },
		{ Decoration::Poly | Decoration::Negative | Decoration::Color::Blue, 4 } }, Decoration::Poly);

	//Full Dots + Arrows
	generator->resetConfig();
	generator->arrowColor = { 0, 0.5f, 0.5f, 1 };
	generator->successColor = { 0, 0.5f, 0.5f, 1 };
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->generate(0x0A16B, Decoration::Dot_Intersection, 36, Decoration::Arrow, 4, Decoration::Stone | Decoration::Color::White, 6, Decoration::Stone | Decoration::Color::Magenta, 5, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x0A2CE, Decoration::Dot_Intersection, 36, Decoration::Arrow, 4, Decoration::Star | Decoration::Color::White, 8, Decoration::Star | Decoration::Color::Magenta, 6, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x0A2D7, Decoration::Dot_Intersection, 36, Decoration::Arrow, 5, Decoration::Poly | Decoration::Can_Rotate, 3, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x0A2DD, Decoration::Dot_Intersection, 36, Decoration::Arrow, 5, Decoration::Star | Decoration::Color::White, 5, Decoration::Star | Decoration::Color::Magenta, 3, Decoration::Poly | Decoration::Color::White | Decoration::Can_Rotate, 2, Decoration::Poly | Decoration::Color::Magenta | Decoration::Can_Rotate, 1, Decoration::Start, 1, Decoration::Exit, 1);
	generator->generate(0x0A2EA, Decoration::Dot_Intersection, 36, Decoration::Arrow, 6, Decoration::Triangle | Decoration::Color::White, 4, Decoration::Star | Decoration::Color::White, 5, Decoration::Stone | Decoration::Color::White, 6, Decoration::Start, 1, Decoration::Exit, 1);
	generator->setGridSize(6, 6);
	generator->pathWidth = 0.85f;
	generator->generate(0x17FB9, Decoration::Dot_Intersection, 49, Decoration::Arrow, 6, Decoration::Star | Decoration::Color::Magenta, 6, Decoration::Stone | Decoration::Color::Magenta, 6, Decoration::Triangle | Decoration::Color::Magenta, 6, Decoration::Start, 1, Decoration::Exit, 1);

	//Invisible Symbols
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteColors);
	generator->setGridSize(3, 3);
	generator->generate(0x0008F, Decoration::Dot_Intersection, 3, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::X, 3);
	generator->setGridSize(4, 4);
	generator->generate(0x0006B, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::X, 3,
		Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::X, 2);
	generator->generate(0x0008B, Decoration::Stone | Decoration::Color::Black, 3, Decoration::Stone | Decoration::Color::X, 2,
		Decoration::Triangle | Decoration::Color::Black, 2, Decoration::Star | Decoration::Color::X, 3);
	generator->generate(0x0008C, Decoration::Star | Decoration::Color::Black, 4, Decoration::Star | Decoration::Color::X, 4,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::X, 1, Decoration::Poly | Decoration::Color::X, 1);
	generator->generate(0x0008A, Decoration::Star | Decoration::Color::Black, 2, Decoration::Triangle | Decoration::Color::Black, 2,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Black, 1, Decoration::Poly | Decoration::Color::X, 1, Decoration::Eraser | Decoration::Color::X, 1);
	generator->setGridSize(0, 0);
	generator->generate(0x00089, Decoration::Poly | Decoration::Color::X, 2, Decoration::Poly | Decoration::Negative | Decoration::Color::X, 4, Decoration::Dot, 8);
	generator->generate(0x0006A, Decoration::Dot, 4, Decoration::Star | Decoration::Color::Black, 4,
		Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::X, 6);
	generator->generate(0x0006C, Decoration::Dot, 6, Decoration::Star | Decoration::Color::Black, 4,
		Decoration::Star | Decoration::Color::Orange, 4, Decoration::Star | Decoration::Color::X, 6,
		Decoration::Eraser | Decoration::Color::X, 1);
	generator->generate(0x00027, Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::X, 2,
		Decoration::Eraser | Decoration::Color::Black, 1);
	generator->generate(0x00028, Decoration::Poly | Decoration::Color::Black, 2, Decoration::Poly | Decoration::Color::X, 2);
	generator->generate(0x00029, Decoration::Triangle | Decoration::Color::Black, 8, Decoration::Eraser | Decoration::Color::X, 1);

	generator->resetConfig();
	//Arrows Exit
	generator->setGridSize(6, 6);
	generator->backgroundColor = { 0.5f, 0.5f, 0.5f, 1 };
	generator->arrowColor = { 0.6f, 0, 1, 1 };
	generator->successColor = { 0.6f, 0, 1, 1 };
	generator->generate(0x17CF2, Decoration::Arrow1, 16);
	//Stars Exit
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->setFlagOnce(Generate::Config::EnableFlash);
	generator->generate(0x021D7, Decoration::Triangle | Decoration::Color::Cyan, 3, Decoration::Triangle | Decoration::Color::Yellow, 3,
		Decoration::Star | Decoration::Color::Cyan, 3, Decoration::Star | Decoration::Color::Yellow, 3,
		Decoration::Eraser | Decoration::Color::Cyan, 1);
	//Arrow Pillar
	generator->resetConfig();
	specialCase->WritePanelData(0x09DD5, PATH_COLOR, { 0.01f, 0, 0.02f, 1 });
	generator->backgroundColor = { 0, 0, 0, 1 };
	generator->arrowColor = { 0.6f, 0, 1, 1 };
	generator->successColor = { 1, 1, 1, 1 };
	generator->setGridSize(6, 5);
	generator->pathWidth = 0.5f;
	generator->generate(0x09DD5, Decoration::Arrow, 7, Decoration::Triangle, 5);
	generator->pathWidth = 1;
	generator->successColor = { 0, 0, 0, 0 };
	//Challenge Entrance
	generator->backgroundColor = { 0.5, 0, 0, 0 };
	generator->arrowColor = { 0, 0.65f, 0, 1 };
	generator->generate(0x0A16E, Decoration::Star | Decoration::Color::Green, 5, Decoration::Arrow | Decoration::Color::Green, 4, Decoration::Stone | Decoration::Color::Green, 5, Decoration::Triangle| Decoration::Color::Green, 5);
	//Theater Exit
	generator->backgroundColor = { 0.5f, 0.5f, 0.5f, 1 };
	generator->arrowColor = { 0.6f, 0, 1, 1 };
	generator->generate(0x039B4, Decoration::Arrow2, 12);
	//Town Exit
	generator->setGridSize(6, 6);
	generator->generate(0x09E85, Decoration::Arrow, 16, Decoration::Start, 4, Decoration::Exit, 1);
}

void PuzzleList::GenerateOrchardH()
{
	specialCase->generateApplePuzzle(0x00143, false, true);
	specialCase->generateApplePuzzle(0x0003B, false, Random::rand() % 2 == 0);
	specialCase->generateApplePuzzle(0x00055, false, Random::rand() % 2 == 0);
	specialCase->generateApplePuzzle(0x032F7, false, Random::rand() % 2 == 0);
	specialCase->generateApplePuzzle(0x032FF, true, true);
}

void PuzzleList::GenerateDesertH()
{
	Randomizer().RandomizeDesert();
}

void PuzzleList::GenerateKeepH()
{
	generator->setLoadingData(L"Keep", 5);
	generator->resetConfig();

	generator->setObstructions({ { 8, 3 },{ 4, 5 },{ 3, 0 },{ 3, 2 },{ 5, 6 } });
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(0x033EA, Decoration::Triangle | Decoration::Color::Yellow, 4, Decoration::Star | Decoration::Color::Yellow, 3, Decoration::Stone | Decoration::Color::Yellow, 2);
	std::set<Point> path1 = generator->_path;
	generator->write(0x033EA);

	generator->resetConfig();
	generator->setObstructions({ { 8, 5 },{ 6, 5 },{ 1, 8 },{ 1, 6 },{ 1, 4 },{ 2, 3 },{ 4, 3 },{ 6, 3 } });
	generator->hitPoints = { { 3, 2 },{ 1, 0 } };
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(0x01BE9, Decoration::Star | Decoration::Color::Black, 3, Decoration::Star | Decoration::Color::White, 3,
		Decoration::Stone | Decoration::Color::Black, 2, Decoration::Stone | Decoration::Color::White, 2,
		Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Black, 1, Decoration::Poly | Decoration::Color::White, 1);
	std::set<Point> path2 = generator->_path;
	generator->write(0x01BE9);

	generator->resetConfig();
	std::vector<std::vector<Point>> validHitPoints = {
		{ { 3, 4 },{ 7, 2 },{ 3, 2 } },{ { 4, 5 },{ 7, 2 },{ 2, 5 } },{ { 4, 3 },{ 7, 2 },{ 2, 3 } },
	{ { 3, 4 },{ 7, 2 },{ 3, 2 } },{ { 1, 4 },{ 7, 2 },{ 1, 2 } },{ { 3, 2 },{ 7, 2 },{ 3, 0 } },{ { 1, 2 },{ 7, 2 },{ 1, 0 } } };
	generator->hitPoints = validHitPoints[Random::rand() % validHitPoints.size()];
	generator->setObstructions({ { 5, 8 } });
	generator->setFlagOnce(Generate::Config::SplitShapes);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(0x01CD3, Decoration::Stone | Decoration::Color::Black, 3, Decoration::Stone | Decoration::Color::White, 2,
		Decoration::Triangle | Decoration::Color::Yellow, 5, Decoration::Poly, 1, Decoration::Poly | Decoration::Can_Rotate, 1
	);
	std::set<Point> path3 = generator->_path;
	generator->write(0x01CD3);

	generator->resetConfig();
	generator->setSymmetry(Panel::Symmetry::None);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->setObstructions({ { 0, 7 },{ 2, 7 },{ 4, 7 },{ 6, 7 },{ 8, 7 },{ 8, 5 },{ 7, 6 } });
	generator->setSymbol(Decoration::Start, 8, 0);
	generator->setSymbol(Decoration::Exit, 8, 4);
	generator->setFlagOnce(Generate::Config::ShortPath);
	generator->generate(0x01D3F, Decoration::Poly | Decoration::Color::Yellow, 2, Decoration::Poly | Decoration::Color::Blue, 1,
		Decoration::Star | Decoration::Color::Yellow, 2, Decoration::Star | Decoration::Color::Blue, 2,
		Decoration::Triangle | Decoration::Color::Yellow, 3, Decoration::Triangle | Decoration::Color::Blue, 3
	);
	generator->_panel->_startpoints.push_back({ 0, 8 });
	generator->_panel->_endpoints.push_back(Endpoint(0, 4, Endpoint::Direction::LEFT, Decoration::Exit));
	if (generator->_panel->_endpoints[0].GetX() == 0) {
		std::swap(generator->_panel->_endpoints[0], generator->_panel->_endpoints[1]); //Need to have endpoints in right order to associate with pressure plates correctly
	}
	std::set<Point> path4 = generator->_path;
	generator->write(0x01D3F);

	specialCase->generateKeepLaserPuzzle(0x03317, path1, path2, path3, path4,
		{ { Decoration::Triangle | Decoration::Color::Yellow, 10 },{ Decoration::Star | Decoration::Color::Yellow, 4 },{ Decoration::Stone | Decoration::Color::Yellow, 2 },
		{ Decoration::Star | Decoration::Color::Black, 3 },{ Decoration::Star | Decoration::Color::White, 3 },
		{ Decoration::Stone | Decoration::Color::Black, 5 },{ Decoration::Stone | Decoration::Color::White, 4 },
		{ Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Black, 1 },{ Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::White, 1 },
		{ Decoration::Poly | Decoration::Can_Rotate | Decoration::Color::Yellow, 2 },{ Decoration::Poly | Decoration::Color::Yellow, 3 },{ Decoration::Poly | Decoration::Color::Blue, 3 },
		{ Decoration::Star | Decoration::Color::Blue, 2 },{ Decoration::Triangle | Decoration::Color::Yellow, 2 },{ Decoration::Triangle | Decoration::Color::Blue, 2 } });

	specialCase->clearTarget(0x0360E); //Must solve pressure plate side
}

void PuzzleList::GenerateJungleH()
{
	generator->setLoadingData(L"Jungle", 6);
	generator->resetConfig();
	specialCase->generateSoundDotPuzzle(0x0026D, { 3, 3 }, { DOT_SMALL, DOT_LARGE }, false);
	specialCase->generateSoundDotReflectionPuzzle(0x0026E, { 5, 5 }, { DOT_SMALL, DOT_LARGE }, { DOT_SMALL, DOT_LARGE }, 0, true);
	specialCase->generateSoundDotReflectionPuzzle(0x0026F, { 7, 7 }, { DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_MEDIUM, DOT_LARGE },
		{ DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_MEDIUM, DOT_LARGE }, 0, true);
	specialCase->generateSoundDotReflectionPuzzle(0x00C3F, { 7, 7 }, { DOT_SMALL, DOT_MEDIUM, DOT_SMALL, DOT_LARGE },
		{ DOT_LARGE, DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_LARGE }, 0, true);
	if (Random::rand() % 2) specialCase->generateSoundDotReflectionPuzzle(0x00C41, { 7, 7 }, { DOT_SMALL, DOT_SMALL, DOT_LARGE, DOT_MEDIUM, DOT_LARGE },
		{ DOT_SMALL, DOT_SMALL, DOT_LARGE, DOT_MEDIUM, DOT_LARGE }, 0, true);
	else specialCase->generateSoundDotReflectionPuzzle(0x00C41, { 7, 7 }, { DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_MEDIUM, DOT_LARGE },
		{ DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_MEDIUM, DOT_LARGE }, 0, true);
	switch (Random::rand() % 4) {
	case 0: specialCase->generateSoundDotReflectionPuzzle(0x014B2, { 7, 7 }, { DOT_SMALL, DOT_LARGE, DOT_SMALL, DOT_LARGE, DOT_MEDIUM },
		{ DOT_SMALL, DOT_LARGE, DOT_SMALL, DOT_LARGE, DOT_MEDIUM }, 0, true); break;
	case 1: specialCase->generateSoundDotReflectionPuzzle(0x014B2, { 7, 7 }, { DOT_LARGE, DOT_MEDIUM, DOT_SMALL, DOT_LARGE, DOT_SMALL },
		{ DOT_LARGE, DOT_MEDIUM, DOT_SMALL, DOT_LARGE, DOT_SMALL }, 0, true); break;
	case 2: specialCase->generateSoundDotReflectionPuzzle(0x014B2, { 7, 7 }, { DOT_LARGE, DOT_MEDIUM, DOT_SMALL, DOT_LARGE, DOT_SMALL },
		{ DOT_SMALL, DOT_LARGE, DOT_SMALL, DOT_LARGE, DOT_MEDIUM }, 0, true); break;
	case 3: specialCase->generateSoundDotReflectionPuzzle(0x014B2, { 7, 7 }, { DOT_SMALL, DOT_LARGE, DOT_SMALL, DOT_LARGE, DOT_MEDIUM },
		{ DOT_LARGE, DOT_MEDIUM, DOT_SMALL, DOT_LARGE, DOT_SMALL }, 0, true); break;
	}
}

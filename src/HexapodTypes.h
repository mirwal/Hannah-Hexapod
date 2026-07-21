// HexapodTypes.h
#pragma once
/**
 * Gemeinsame Bedeutungsnamen für einfache Hexapod-Datentypen.
 *
 * Diese Datei sammelt Typ-Aliase, die technisch auf allgemeinen Grundtypen
 * wie Vector3D beruhen, im Code aber eine fachliche Bedeutung bekommen.
 *
 * Beispiel: LegPosition ist technisch ein Vector3D, beschreibt im Kontext
 * von HANNAH aber eine Position, die zu einem Bein gehört.
 */

#define sendDebug(args) (Serial.print(args))
#define sendDebugln(args) (Serial.println(args))

#include "Vector3D.h"
using LegPositionLocal = Vector3D;  // Fußziel relativ zum Coxa-Montagepunkt
using FootPositionBody = Vector3D;  // Fußposition im Body-Koordinatensystem
using MountPositionBody = Vector3D; // Coxa-Montagepunkt im Body-Koordinatensystem
using MoveVectorBody = Vector3D;    // Bewegungsrichtung im Body-Koordinatensystem
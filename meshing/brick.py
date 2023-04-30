#!/usr/bin/env python

###
### This file is generated automatically by SALOME v9.9.0 with dump python functionality
###

import sys
import salome

salome.salome_init()
import salome_notebook
notebook = salome_notebook.NoteBook()
sys.path.insert(0, r'/home/anders/dev/Compress3D/meshing')

###
### SHAPER component
###

from salome.shaper import model

model.begin()
partSet = model.moduleDocument()
model.end()

###
### SHAPERSTUDY component
###

###
### GEOM component
###

import GEOM
from salome.geom import geomBuilder
import math
import SALOMEDS


geompy = geomBuilder.New()

O = geompy.MakeVertex(0, 0, 0)
OX = geompy.MakeVectorDXDYDZ(1, 0, 0)
OY = geompy.MakeVectorDXDYDZ(0, 1, 0)
OZ = geompy.MakeVectorDXDYDZ(0, 0, 1)
Box_1 = geompy.MakeBoxDXDYDZ(30, 10, 10)
sides = geompy.CreateGroup(Box_1, geompy.ShapeType["FACE"])
geompy.UnionIDs(sides, [23, 33, 31, 27])
outlet = geompy.CreateGroup(Box_1, geompy.ShapeType["FACE"])
geompy.UnionIDs(outlet, [13])
inlet = geompy.CreateGroup(Box_1, geompy.ShapeType["FACE"])
geompy.UnionIDs(inlet, [3])
[sides, outlet, inlet] = geompy.GetExistingSubObjects(Box_1, False)
geompy.addToStudy( O, 'O' )
geompy.addToStudy( OX, 'OX' )
geompy.addToStudy( OY, 'OY' )
geompy.addToStudy( OZ, 'OZ' )
geompy.addToStudy( Box_1, 'Box_1' )
geompy.addToStudyInFather( Box_1, sides, 'sides' )
geompy.addToStudyInFather( Box_1, outlet, 'outlet' )
geompy.addToStudyInFather( Box_1, inlet, 'inlet' )

###
### SMESH component
###

import  SMESH, SALOMEDS
from salome.smesh import smeshBuilder

smesh = smeshBuilder.New()
#smesh.SetEnablePublish( False ) # Set to False to avoid publish in study if not needed or in some particular situations:
                                 # multiples meshes built in parallel, complex and numerous mesh edition (performance)

Mesh_1 = smesh.Mesh(Box_1,'Mesh_1')
NETGEN_1D_2D_3D = Mesh_1.Tetrahedron(algo=smeshBuilder.NETGEN_1D2D3D)
sides_1 = Mesh_1.GroupOnGeom(sides,'sides',SMESH.FACE)
outlet_1 = Mesh_1.GroupOnGeom(outlet,'outlet',SMESH.FACE)
inlet_1 = Mesh_1.GroupOnGeom(inlet,'inlet',SMESH.FACE)
isDone = Mesh_1.Compute()
[ sides_1, outlet_1, inlet_1 ] = Mesh_1.GetGroups()


## Set names of Mesh objects
smesh.SetName(outlet_1, 'outlet')
smesh.SetName(inlet_1, 'inlet')
smesh.SetName(sides_1, 'sides')
smesh.SetName(NETGEN_1D_2D_3D.GetAlgorithm(), 'NETGEN 1D-2D-3D')
smesh.SetName(Mesh_1.GetMesh(), 'Mesh_1')


if salome.sg.hasDesktop():
  salome.sg.updateObjBrowser()

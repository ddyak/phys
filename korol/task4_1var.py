import numpy as np
import ROOT
import Geant4
from Geant4 import cm, mm, MeV, m
import g4py.EMSTDpl
import g4py.NISTmaterials
import g4py.ezgeom
import g4py.ParticleGun
import math
import sys
from ROOT import TCanvas, TColor, TGaxis, TH1F, TPad

# electron/gamma standard EM physics list
g4py.EMSTDpl.Construct()

c = TCanvas("c", "canvas", 500, 1200)
iterations = 1000.0

class MySD(Geant4.G4VSensitiveDetector):

	def __init__(self):
		
		Geant4.G4VSensitiveDetector.__init__(self, "MySD")
		self.eventEnergy = 0.0
		self.count = 0
		self.x = []
		self.y = []

	def ProcessHits(self, step, rohist):
		
		energy = step.GetTotalEnergyDeposit() / MeV
		self.eventEnergy += energy
		self.count += 1		
    		self.x.append(step.GetPreStepPoint().GetPosition().x * energy)
		self.y.append(step.GetPreStepPoint().GetPosition().y * energy)


class MyEventAction(Geant4.G4UserEventAction):

	def __init__(self, sd_pb, sd_au, hist, hist1, hist2):
	
		Geant4.G4UserEventAction.__init__(self)
		self.sd_pb = sd_pb
		self.sd_au = sd_au

		self.hist_pb_energy = hist
		self.hist_au_energy = hist1
		self.hist_coord = hist2

		self.count = 0
		self.counter_pb = 0
		self.counter_au = 0

	def BeginOfEventAction(self, event):
		
		self.count += 1
		self.sd_pb.eventEnergy = 0
		self.sd_au.eventEnergy = 0
		self.sd_pb.count = 0
		self.sd_au.count = 0
		self.sd_au.x = []
		self.sd_au.y = []

	def EndOfEventAction(self, event):
		
		if self.sd_pb.count != 0:
			self.counter_pb += 1
		if self.sd_au.count != 0:
			self.counter_au += 1
		
		self.hist_pb_energy.Fill(self.sd_pb.eventEnergy / MeV)
		self.hist_au_energy.Fill(self.sd_au.eventEnergy / MeV)

		if self.sd_au.count > 0:
			xAvg = np.sum(self.sd_au.x) / self.sd_au.eventEnergy / mm
			yAvg = np.sum(self.sd_au.y) / self.sd_au.eventEnergy / mm
			self.hist_coord.Fill(xAvg, yAvg, self.sd_au.eventEnergy / MeV)
		self.sd_au.count = 0


g4py.NISTmaterials.Construct()
g4py.ezgeom.Construct()
# fill the world with air
air = Geant4.G4Material.GetMaterial("G4_AIR")
g4py.ezgeom.SetWorldMaterial(air)
g4py.ezgeom.ResizeWorld(5.0 * m, 5.0 * m, 6.0 * m)

# Plate
Au = Geant4.G4Material.GetMaterial("G4_Au")
target_plate = g4py.ezgeom.G4EzVolume("Aurum_plate")
target_plate.CreateBoxVolume(Au, 10.0 * cm, 10.0 * cm, 1.0 * mm)

detector_plate = MySD()
target_plate.SetSensitiveDetector(detector_plate)
target_plate.PlaceIt(Geant4.G4ThreeVector(0.0, 0.0, 1.0 * m))

# Cube
Pb = Geant4.G4Material.GetMaterial("G4_Pb")
target = g4py.ezgeom.G4EzVolume("Plumbum_cube")
target.CreateBoxVolume(Pb, 1.0 * m, 1.0 * m, 1.0 * m)

detector_cube = MySD()
target.SetSensitiveDetector(detector_cube)
target.PlaceIt(Geant4.G4ThreeVector(0.0, 0.0, 1.5 * m))


# гистограммы
hist_au = ROOT.TH1D("hist_au", "Energy_au", 70, -1, 12.0)
hist_pb = ROOT.TH1D("hist_pb", "Energy_pb", 70, -1, 12.0) 
hist_coordinates = ROOT.TH2D("hist_coord", "coordinatess", 50, -100, 50, 100, -100, 100)

uaction = MyEventAction(detector_cube, detector_plate, hist_pb, hist_au, hist_coordinates)
Geant4.gRunManager.Initialize()
Geant4.gRunManager.SetUserAction(uaction)

# включить рисование
Geant4.gApplyUICommand("/control/execute visinit.mac")
Geant4.gApplyUICommand('/vis/enable true')
# создать первичный генератор
pg = g4py.ParticleGun.Construct()
# установить постоянные параметры первичного генератора
pg.SetParticleByName("gamma") # тип частицы
pg.SetParticlePosition( Geant4.G4ThreeVector(0.0, 0.0, 0.0) ) # точка вылета
pg.SetParticleEnergy(10.0*MeV) # энргия
# цикл по событиям
for i in range(0, iterations):
	
	if (i == 0):
		Geant4.gApplyUICommand('/vis/enable false')
	phi = ROOT.gRandom.Uniform(0.0, 2.0 * math.pi) # равномерно по фи
	#theta = ROOT.gRandom.Uniform(0.0, 5.0 * math.pi / 180) # разброс 5 градусов по theta
	#xcos = ROOT.gRandom.Uniform(0, math.cos(5.0 * math.pi / 180))
	#theta = ROOT.gRandom.Uniform(0.0, math.acos(xcos) * math.pi / 180)
	theta = math.acos(ROOT.gRandom.Uniform(math.cos(10.0 * math.pi / 180), math.cos(0.0 * math.pi / 180)))
	vx = math.sin(theta) * math.cos(phi)
	vy = math.sin(theta) * math.sin(phi)
	vz = math.cos(theta)
	pg.SetParticleMomentumDirection(Geant4.G4ThreeVector(vx, vy, vz)) # направление
	Geant4.gRunManager.BeamOn(1) # проведение частицы

#sys.stdin.readline()
print(uaction.counter_au, uaction.counter_pb, iterations)
hist_pb.Draw()

sys.stdin.readline()
hist_au.Draw()

sys.stdin.readline()
hist_coordinates.Draw('lego')

sys.stdin.readline()

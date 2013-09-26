io.write("Running ", _VERSION, "\n")

importMap("maps/tutorial.map")
createPlayer(1.0,0.0,0.0)
createPlayer(1.0,0.55,0.05)

createUnit(8*64, 12*64, 41, 1)
printUnits()
	
createBuilding(5, 5, 21, 0, 0)
createBuilding(102, 102, 21, 1, 0)
printBuildings()

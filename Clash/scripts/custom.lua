io.write("Running ", _VERSION, "\n")
importMap("maps/custom.map")

-- create players
createPlayer(1.0,0.0,0.0)
--createPlayer(1.0,0.55,0.05)
--createPlayer(0.0,0.8,0.07)
createPlayer(0.0,0.4,0.7)
	
-- create command centers
createBuilding(5, 5, 21, 0, 0)
-- createBuilding(102, 102, 21, 1, 0)
-- createBuilding(5, 102, 21, 2, 0)
-- createBuilding(102, 5, 21, 3, 0)

-- color tests
-- createBuilding(4, 18, 23, 0, 0)
-- createBuilding(7, 18, 23, 1, 0)
-- createBuilding(10, 18, 23, 2, 0)
-- createBuilding(13, 18, 23, 3, 0)

-- createBuilding(4, 12, 22, 0, 0)
-- createBuilding(4, 15, 22, 1, 0)
-- createBuilding(7, 12, 22, 2, 0)
-- createBuilding(7, 15, 22, 3, 0)

-- createBuilding(10, 12, 21, 0, 0)
-- createBuilding(10, 15, 21, 1, 0)
-- createBuilding(13, 12, 21, 2, 0)
-- createBuilding(13, 15, 21, 3, 0)

-- For kolloqium
createBuilding(45, 5, 21, 1, 0)
createBuilding(42, 9, 22, 1, 0)
createBuilding(40, 4, 23, 1, 0)
createBuilding(48, 10, 23, 1, 0)

createUnit(35*64, 6*64, 41, 1, 0)
createUnit(35*64+42, 6*64, 41, 1, 0)
createUnit(35*64+20, 6*64+42, 41, 1, 0)

createUnit(50*64, 4*64, 42, 1, 0)
createUnit(48*64, 3*64, 42, 1, 0)
createUnit(49*64, 2*64, 42, 1, 0)

--moveUnit(a, 6*64, 2*64)

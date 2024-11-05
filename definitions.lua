local bullet = {
	size = 20,
	shape = 0,
	body = { health = 10 },
}

local aggressor = {
	size = 32.5,
	shape = 0,
	guns = {
		{ length = 30, width = 17.5, aspect = 1, body = { bspeed = 2.5, reload = 10, life = 50 } },
	},
	body = { health = 50, speed = 2 },
}

Exports = { aggressor, bullet }

local counter = 0
for _, value in ipairs(Exports) do
	value.id = counter
	counter = counter + 1
end

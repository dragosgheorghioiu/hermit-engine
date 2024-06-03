function on_key_press()
	-- print("on_key_press")
	-- trigger_event("PluginEvent", 100, 200, "a")
end

function temp(entity, component)
	print(entity:get_id())
	print(entity:has_component("TransformComponent"))
	print(component.name)
end

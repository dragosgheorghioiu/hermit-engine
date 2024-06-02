function on_key_press()
	print("on_key_press")
	trigger_event("PluginEvent", 100, 200, "a")
end

on_key_press()

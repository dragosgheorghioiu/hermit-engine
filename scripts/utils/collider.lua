collider = {
	box_collider_limit_from_transform = function(entity)
		local box_collider_component = entity:get_component("BoxColliderComponent")
		local transform_component = entity:get_component("TransformComponent")

		local entity_left = get_transform_position_x(transform_component)
			+ get_box_collider_position_x(box_collider_component) * get_transform_scale_x(transform_component)
		local entity_right = entity_left
			+ get_box_collider_width(box_collider_component) * get_transform_scale_x(transform_component)
		local entity_top = get_transform_position_y(transform_component)
			+ get_box_collider_position_y(box_collider_component) * get_transform_scale_y(transform_component)
		local entity_bottom = entity_top
			+ get_box_collider_height(box_collider_component) * get_transform_scale_y(transform_component)

		return { entity_left, entity_top, entity_right, entity_bottom }
	end,
}

return collider

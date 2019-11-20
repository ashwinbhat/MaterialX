void mx_angle_unit_vector2(vec2 _in, int _unit_from, int _unit_to, out vec2 result)
{
    result = _in * mx_angle_unit_ratio(_unit_from, _unit_to);
}

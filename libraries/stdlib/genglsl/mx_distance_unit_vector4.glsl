void mx_distance_unit_vector4(vec4 _in, int _unit_from, int _unit_to, out vec4 result)
{
    result = _in * mx_distance_unit_ratio(_unit_from, _unit_to);
}

program Cycle;
label a;
var
	i : integer;
begin
	i := 10;
	a:
if i >= 0 then
	begin
		write(i);
		writeln();
		i := i - 1;
		goto a;
end;
	write(' hi');
end.

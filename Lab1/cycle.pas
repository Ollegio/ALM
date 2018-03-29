program Cycle;
var
	i : integer;
begin
	i := 10;
	while i >= 0 do
	begin
		write(i);
		writeln();
		i := i - 1;
	end;
	write(' hi');
end.

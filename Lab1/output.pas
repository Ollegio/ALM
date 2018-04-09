program Cycle;
label cycle;
var
	k : integer;
begin
	k := 10;
	cycle:
if k < 100 then
	begin
		write(i);
		writeln();
		i := i - 1;
		goto cycle;
end;
	write(' hi');
end.

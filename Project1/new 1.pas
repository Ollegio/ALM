program Cycle;
var
	i, a : integer;
label
	C1;
begin
	i := 0;
C1:
	if i < 10 then
	begin
		a := a + 5;
		i := i + 1;
		goto C1;
	end;
	writeln(a);
end.
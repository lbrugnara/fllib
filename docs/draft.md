import std.out;

func greet (string name) -> int 
{
	println("Hello {0}!", name)
	return 0
}


func main -> int
{
	greet("Leo") -> result;
	return result;
}

func main
{
	greet("Leo")
}

func main (string[] argv, int argc) -> int
{
	greet(argv[0]) -> result;
	return result;
}
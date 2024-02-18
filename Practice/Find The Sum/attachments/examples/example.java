import java.util.Scanner;
import java.util.ArrayList;
import java.util.ArrayList;

class Main
{
	Scanner scanner;
	int n;
	long sum=0;

	Main()
	{
	    scanner=new Scanner(System.in);
	    //Reading input
	    n=scanner.nextInt();
	}
	
	long query(int i, int j)
	{
		System.out.println(String.format("? %d %d",i,j));
		System.out.flush();
		return this.scanner.nextLong();
	}

	void answer()
	{
		System.out.println("! " + sum);
		System.out.flush();
	}
	

	public static void main(String args[])
	{
		Main solver = new Main();
		/*
			Your code here
		*/
	 	
	}
}

import java.util.Scanner;
import java.util.ArrayList;
import java.util.ArrayList;

class Main
{
	Scanner scanner;
	int n;
	ArrayList<Integer> permutation;
	
	Main()
	{
	    scanner=new Scanner(System.in);
	    //Reading input
	    n=scanner.nextInt();
	    permutation = new ArrayList<Integer>(n);
	    //The permutation is unknown, initialize it to zero. You have to find it!
	    for(int i=0;i<n;i++)
	        permutation.add(0);
	}
	
	long query(int i, int j)
	{
		System.out.println(String.format("? %d %d",i,j));
		System.out.flush();
		return this.scanner.nextLong();
	}

	void answer()
	{
		System.out.print("! ");
		for (var p : this.permutation)
			System.out.print(String.format("%d ",p));
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

import java.io.*;
import java.util.Scanner;
import java.util.ArrayList;

public class interactor
{

    static boolean is_valid(int i, int j, int n)
    {
        return i>=0 && i<n && j>=0 && j<n && i!=j;
    }

    public static void main(String... args)
    {
        if(args.length==0)
        {
            System.err.println("Usage interactor <output_file>");
            System.exit(1);
        }
        //Get environment variable NDEBUG
        String NDEBUG = System.getenv("NDEBUG");
        String DEBUG = System.getenv("DEBUG");
        PrintStream bis=null;
        if(NDEBUG==null && DEBUG != null)
        {
            try
            {
                bis=new PrintStream(new BufferedOutputStream(new FileOutputStream(DEBUG)));
            }
            catch(Exception e)
            {
                System.err.println("Error: "+e.getMessage());
            }
        }

        BufferedInputStream stream=null;
        int n;
        ArrayList<Integer> P = new ArrayList<Integer>();
        long sum=0;
        try
        {
            stream = new BufferedInputStream(new FileInputStream(args[0]));
            Scanner inScanner=new Scanner(stream);
            n=inScanner.nextInt();
            for(int i = 0; i < n; i++)
                P.add(inScanner.nextInt());
            for (Integer p:P)
                sum+=p;
        }
        catch(Exception e)
        {
            System.err.println("Error: "+e.getMessage());
            System.exit(1);
            return;
        }
        int queries=0;
        Scanner sc = new Scanner(System.in);
        System.out.println(n);
        System.out.flush();

        String command="";
        while (!command.equals("!"))
        {
            command = sc.next();
            if(command.equals("!"))
            {
                long sum_guess=sc.nextLong();
                if(bis!=null)
                {
                    bis.print("Received input: ! ");
                    bis.println(sum_guess);
                    bis.flush();
                }
                if(sum==sum_guess)
                    System.err.println(String.format("Solution guessed the correct permutation! Number of queries: %d",queries));
                else
                {
                    System.err.println("Solution guessed the wrong permutation!");
                    System.err.println("Expected: " + sum);
                    System.err.println("Found:    " + sum_guess);
                    System.err.print("Array: ");
                    for(int i=0;i<n;i++)
                        System.err.print((i>0?" ":"")+P.get(i));
                    System.err.println();
                    System.err.flush();
                }
            }
            else if(command.equals("?"))
            {
                queries++;
                int a = sc.nextInt();
                int b = sc.nextInt();
                if(!is_valid(a-1,b-1,n))
                {
                    System.err.println(String.format("Invalid query: ? %d %d",a,b));
                    System.exit(0);
                }
                if(bis!=null)
                {
                    bis.println(String.format("Received input: ? %d %d", a, b));
                    bis.flush();
                }
                System.out.println(P.get(a-1)+P.get(b-1));
                System.out.flush();

                if(bis!=null)
                {
                    bis.println(String.format("Sent output: %d",P.get(a-1)+P.get(b-1)));
                    bis.flush();
                }
            }
            else
            {
                System.err.println("Invalid command: "+command);
                System.exit(0);
            }
        }
    }
}

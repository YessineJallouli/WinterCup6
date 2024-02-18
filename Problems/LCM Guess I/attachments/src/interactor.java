import java.io.*;
import java.util.Scanner;
import java.util.ArrayList;

public class interactor
{
    static long gcd(long a , long b)
    {
        if(a<b)
            return gcd(b,a);
        if(b==0)
            return a;
        return gcd(b,a%b);
    }

    static long lcm(long a, long b)
    {
        return a/gcd(a,b)*b;
    }

    static boolean is_valid(int i, int j, int n)
    {
        return i>=0 && i<n && j>=0 && j<n && i!=j;
    }

    static boolean is_permutation(ArrayList<Integer> P)
    {
        int n = P.size();
        boolean[] used = new boolean[n];
        for(int i = 0; i < n; i++)
        {
            if(P.get(i)<1 || P.get(i)>n || used[P.get(i)-1])
                return false;
            used[P.get(i)-1] = true;
        }
        return true;
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
        try
        {
            stream = new BufferedInputStream(new FileInputStream(args[0]));
            Scanner inScanner=new Scanner(stream);
            n=inScanner.nextInt();
            for(int i = 0; i < n; i++)
                P.add(inScanner.nextInt());
            if(!is_permutation(P))
            {
                System.err.println("Invalid test data. Array is not a permutation.");
                System.exit(1);
                return;
            }
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
                String line= sc.nextLine();
                String[] tokens = line.split(" ");
                ArrayList<Integer> P_guess=new ArrayList<Integer>();
                for(int i=0;i<tokens.length;i++) if(!tokens[i].isEmpty())
                    P_guess.add(Integer.parseInt(tokens[i]));
                if(bis!=null)
                {
                    bis.print("Received input: !");
                    for(int i=0;i<n;i++)
                        bis.print(" "+P_guess.get(i));
                    bis.println();
                    bis.flush();
                }
                if(P.equals(P_guess))
                    System.err.println(String.format("Solution guessed the correct permutation! Number of queries: %d",queries));
                else
                {
                    System.err.println("Solution guessed the wrong permutation!");
                    System.err.println("Expected: ");
                    for(int i=0;i<n;i++)
                        System.err.print((i>0?" ":"")+P.get(i));
                    System.err.println();
                    System.err.println("Found:    ");
                    for(int i=0;i<n;i++)
                        System.err.print((i>0?" ":"")+P_guess.get(i));
                    System.err.println();
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
                System.out.println(lcm(P.get(a-1),P.get(b-1)));
                System.out.flush();

                if(bis!=null)
                {
                    bis.println(String.format("Sent output: %d",lcm(P.get(a-1),P.get(b-1))));
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

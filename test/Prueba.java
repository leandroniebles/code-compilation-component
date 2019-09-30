import java.util.*;

public class Prueba{
  public static void main(String[] args){
    //System.out.println("Hello, World!");
    Scanner scanner = new Scanner(System.in);
    /*    
    String nombre = scanner.nextLine();
    System.out.println("Hello, " + nombre + "!");
    */
    int a = Integer.parseInt(scanner.nextLine());
    int b = Integer.parseInt(scanner.nextLine());
    System.out.println("La suma de a + b es " + (a+b) + "");
    scanner.close();
  }
}

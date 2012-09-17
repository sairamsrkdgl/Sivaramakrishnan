package mhealth;
import java.sql.*;
public class Database {
    public static Connection getConnection() throws Exception {
        
        Class.forName("com.mysql.jdbc.Driver");
        Connection con=DriverManager.getConnection("jdbc:mysql://localhost:3306/mhealthdefault","mhealth","mhealth");
        /*Class.forName("oracle.jdbc.driver.OracleDriver");
        Connection con = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:XE","mhealth","mhealth");*/
        return con;
    }
    public static void close(Connection con) {
        try        {
            con.close();
        }
        catch(Exception ex) {
            System.out.println(ex.getMessage());
        }
    }
}

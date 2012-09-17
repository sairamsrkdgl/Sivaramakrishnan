package mhealth;
import java.sql.*;
public class Doctor {
private String id,dname,specialization,daddress,dcontactno,dpass;
 //private int age; because we're not calculating the age from dob
    public Doctor() {
    }
    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }
   
    public String getDname()
    {
        return dname;
    }
    public void setDname(String dname)
    {
        this.dname= dname;
    }
    public String getDpass()
    {
        return dpass;
    }
    public void setDpass(String dpass)
    {
        this.dpass= dpass;
    }
    public String getSpecialization()
    {
        return specialization;
    }
    public void setSpecialization(String specialization)
    {
        this.specialization= specialization;
    }
    public String getDaddress()
    {
        return daddress;
    }
    public void setDaddress(String daddress)
    {
        this.daddress= daddress;
    }
    public String getDcontactno()
    {
        return dcontactno;
    }
    public void setDcontactno(String dcontactno)
    {
        this.dcontactno= dcontactno;
    }
     public String add() {
      Connection con = null; 
      try {
          con = Database.getConnection();
          Statement st = con.createStatement();
          // get patient id
          ResultSet rs = st.executeQuery("select ifnull(max(doctor_id),5000) + 1 from doctors");
          rs.next();
          id = rs.getString(1);
          rs.close();
          
          PreparedStatement ps = con.prepareStatement("insert into doctors values(?,?,?,?,?,?)");
          ps.setString(1,id);
          ps.setString(2,dname);
          ps.setString(3,specialization);
          ps.setString(4,daddress);
          ps.setString(5,dcontactno);
          ps.setString(6,dpass);
          if ( ps.executeUpdate() == 1)
              return id;
          else
              return null; // error
      }
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
   } // add()
}

    
     

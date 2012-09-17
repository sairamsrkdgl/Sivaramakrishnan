package mhealth;
import java.sql.*;
public class PatientDependent {
private String pdname,pdaddress,pdcontactno,patientdidpf,id;
 //private int age; because we're not calculating the age from dob
    public PatientDependent() {
        
    }
    public String getPdname()
    {
        return pdname;
    }
    public void setPdname(String pdname)
    {
        this.pdname= pdname;
    }
   
    public String getPdaddress()
    {
        return pdaddress;
    }
    public void setPdaddress(String pdaddress)
    {
        this.pdaddress= pdaddress;
    }
    public String getPdcontactno()
    {
        return pdcontactno;
    }
    public void setPdcontactno(String pdcontactno)
    {
        this.pdcontactno= pdcontactno;
    }
    
    public String getPatientdidpf()
    {
        return patientdidpf;
    }
    public void setPatientdidpf(String patientdidpf)
    {
        this.patientdidpf= patientdidpf;
    }
    
     public String add() {
      Connection con = null; 
      try {
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("insert into patient_dependent values(?,?,?,?)");
          ps.setString(1,patientdidpf);
          ps.setString(2,pdname);
          ps.setString(3,pdaddress);
          ps.setString(4,pdcontactno);
          if ( ps.executeUpdate() == 1)
              return patientdidpf;
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
 
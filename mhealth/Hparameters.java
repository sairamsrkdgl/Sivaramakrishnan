package mhealth;
import java.sql.*;
public class Hparameters {
private String bgroup,hrate,btemp,glevel,bpressure,description,hdate,patientdidpf,pid,access;
private String id;
 //private int age; because we're not calculating the age from dob
    public Hparameters(){
        
    }
     public String getBgroup()
    {
        return bgroup;
    }
    
    public void setBgroup(String bgroup)
    {
            this.bgroup = bgroup;
    }
    
    public String getHrate()
    {
        return hrate;
    }
    
    public void setHrate(String hrate)
    {
        this.hrate = hrate;
    }
    
    public String getBtemp()
    {
        return btemp;
    }
    public void setBtemp(String btemp)
    {
        this.btemp = btemp;
    }
    
    public String getGlevel()
    {
        return glevel;
    }
    public void setGlevel(String glevel)
    {
        this.glevel = glevel;
    }
    public String getBpressure()
    {
        return bpressure;
    }
    public void setBpressure(String bpressure)
    {
        this.bpressure = bpressure;
    }
    
    public String getDescription()
    {
        return description;
    }
    
    public void setDescription(String description)
    {
        this.description = description;
    }
    public String getHdate()
    {
        return hdate;
    }
    public void setHdate(String hdate)
    {
        this.hdate = hdate;
    }
     public String getPatientdidpf()
    {
        return patientdidpf;
    }
    public void setPatientdidpf(String patientdidpf)
    {
        this.patientdidpf= patientdidpf;
    }
    
     public String getPid()
    {
        return pid;
    }
    public void setPid(String pid)
    {
        this.pid= pid;
    }
       
     public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }


     public String add() {
      Connection con = null; 
      description = "To be filled by doctor";
      access ="y";
      try {
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("insert into hparameters values(?,?,?,?,?,?,?,sysdate(),?)");
          ps.setString(1,pid);
          ps.setString(2,bgroup);
          ps.setString(3,hrate);
          ps.setString(4,btemp);
          ps.setString(5,glevel);
          ps.setString(6,bpressure);
          ps.setString(7,description);
          ps.setString(8,access);          
          System.out.println(pid);
          if ( ps.executeUpdate() == 1)
              return pid;
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
     
    public String add1() {
      Connection con = null; 
      try {
          con = Database.getConnection();
          Statement st = con.createStatement();
          access = "y";
          description = "To be filled by doctor";
          // get patient id
          ResultSet rs = st.executeQuery("select ifnull(max(hid),0) + 1 from hparameters");
          rs.next();
          id = rs.getString(1);
          rs.close();
          PreparedStatement ps = con.prepareStatement("insert into hparameters values(?,?,?,?,?,?,?,?,sysdate(),?)");
          ps.setString(1,id);
          ps.setString(2,pid);
          ps.setString(3,bgroup);
          ps.setString(4,hrate);
          ps.setString(5,btemp);
          ps.setString(6,glevel);
          ps.setString(7,bpressure);
          ps.setString(8,description);
          ps.setString(9,access);
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

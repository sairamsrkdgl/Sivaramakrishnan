package mhealth;
import java.sql.*;
import java.util.ArrayList;
public class Patient {
private String id,id1,pname,age,paddress,gender,pcontactno,bgroup,hrate,btemp,glevel,bpressure,description,doctorid,dname,specialization,daddress,dcontactno,doctoridpf,healthl,hid, hdate,patientp,p_access;
static String bloodgroupaccess;
String userName="";
String password="";
 //private int age; because we're not calculating the age from dob
private ArrayList<HealthL> healthlist;
    public Patient() {
    }

    public String getUserName() {
        return userName;
        }
    public void setUserName(String userName) {
            this.userName = userName;
        }
    public String getPassword() {
            return password;
        }
    public void setPassword(String password) {
        this.password = password;
  }
    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

     public String getId1() {
        return id1;
    }

    public void setId1(String id1) {
        this.id1 = id1;
    }

    public String getPname() {
        return pname;
    }

    public void setPname(String pname) {
        this.pname = pname;
    }
    //Setter and getter for patient added new    
    public String getPatientp() {
        return patientp;
    }

    public void setPatientp(String patientp) {
        this.patientp = patientp;
    }
    //
    public String getAge() {
        return age;
    }

    public void setAge(String age) {
        this.age = age;
    }

    public String getPaddress() {
        return paddress;
    }

    public void setPaddress(String paddress) {
        this.paddress = paddress;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public String getPcontactno() {
        return pcontactno;
    }

    public void setPcontactno(String pcontactno) {
        this.pcontactno = pcontactno;
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
    public String getDoctorid()
    {
        return doctorid;
    }
    
    public void setDoctorid(String doctorid)
    {
        this.doctorid = doctorid;
    }
    public String getDname()
    {
        return dname;
    }
    public void setDname(String dname)
    {
        this.dname=dname;
    }
    public String getSpecialization()
    {
        return specialization;
    }
    public void setSpecialization(String specialization)
    {
        this.specialization=specialization;
    }
    public String getDaddress()
    {
        return daddress;
    }
    public void setDaddress(String daddress)
    {
        this.daddress=daddress;
    }
    public String getDcontactno()
    {
        return dcontactno;
    }
    public void setDcontactno(String dcontactno)
    {
        this.dcontactno=dcontactno;
    }
    
   public String getDoctoridpf()
    {
        return doctoridpf;
    }
    public void setDoctoridpf(String doctoridpf)
    {
        this.doctoridpf = doctoridpf;
    }
    public String getHid()
    {
        return hid;
    }
    public void setHidd(String hid)
    {
        this.hid = hid;
    }
    
    public String getHdate()
    {
        return hdate;
    }
    public void setHdate(String hdate)
    {
        this.hdate = hdate;
    }
    public ArrayList<HealthL> getHealthl() {
        return healthlist;
    }

    public void setHealthl(ArrayList<HealthL> healthlist) {
        this.healthlist = healthlist;
    }
    public String add() {
      Connection con = null; 
      try {
          con = Database.getConnection();
          Statement st = con.createStatement();
          p_access = "null";
          // get patient id
          ResultSet rs = st.executeQuery("select ifnull(max(patient_id),1000) + 1 from patients");
          rs.next();
          id = rs.getString(1);
          rs.close();
          PreparedStatement ps = con.prepareStatement("insert into patients values(?,?,?,?,?,?,?,?,?)");
          ps.setString(1,id);
          ps.setString(2,pname);
          ps.setString(3,age);
          ps.setString(4,gender);
          ps.setString(5,paddress);
          ps.setString(6,pcontactno);
          ps.setString(7,doctoridpf);
          ps.setString(8,patientp);
          ps.setString(9,p_access);
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

   
     public String patientadd(String asgdoctor_id) {
      Connection con = null; 
      try {
          con = Database.getConnection();
          Statement st = con.createStatement();
          p_access = "n";
          // get patient id
          ResultSet rs = st.executeQuery("select max(patient_id) from patients");
          rs.next();
          id = rs.getString(1);
          rs.close();
          
          ResultSet rs1 = st.executeQuery("select ifnull(max(aid),0) + 1 from assigned_doctors");
          rs1.next();
          id1 = rs1.getString(1);
          rs1.close();
          
          PreparedStatement ps = con.prepareStatement("insert into assigned_doctors values(?,?,?,?)");
          ps.setString(1,id1);
          ps.setString(2,id);
          ps.setString(3,asgdoctor_id);
          ps.setString(4,p_access);
          if ( ps.executeUpdate() == 1)
              return id1;
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
   } // patientadd()

      
     public static boolean update(String patient_id, String contactno, String address) {
         Connection con = null; 
      try {
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("update patients set p_contactno =?, p_address = ? where patient_id = ?");
          ps.setString(1,contactno);
          ps.setString(2,address);
          ps.setString(3,patient_id);
          if ( ps.executeUpdate() == 1)
              return true;
          else
              return false; // error
      }
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return false;
      }
      finally {
          Database.close(con);
      }
   } // update()

    
    public static Patient getAllDetails(String patient_id) {
      // connect and get all details of the given patient
      Connection con= null;
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select p_name, age, gender, p_address, p_contactno  from patients where patient_id = ?");
          ps.setString(1,patient_id);
          ResultSet rs = ps.executeQuery();
          if (!rs.next () )
              return null;
          System.out.println("Patient Found");
          // create Patient object and load data from row to object
          Patient p = new Patient();
          p.setId(patient_id);
          p.setPname( rs.getString("p_name"));
          p.setAge( rs.getString("age"));
          p.setGender( rs.getString("gender"));
          p.setPaddress( rs.getString("p_address"));
          p.setPcontactno( rs.getString("p_contactno"));
          rs.close();
          return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getAllDetails()
    
    public static Patient getAllHDetails(String patient_id) {
      // connect and get all details of the given patient
      Connection con= null;
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select patient_id, bloodgroup, heart_rate, body_temperature, glucose_level, blood_pressure, description, hdate from hparameters where patient_id = ?");
          ps.setString(1,patient_id);
          ResultSet rs = ps.executeQuery();
          if(! rs.next())
              return null;
          // create Patient object and load data from row to object
          Patient p = new Patient();
          //p.setHidd(rs.getString("hid"));
          p.setId(patient_id);
          //p.setDoctorid(rs.getString("doctor_id"));
          p.setBgroup( rs.getString("bloodgroup"));
          p.setHrate( rs.getString("heart_rate"));
          p.setBtemp( rs.getString("body_temperature"));
          p.setGlevel( rs.getString("glucose_level"));
          p.setBpressure( rs.getString("blood_pressure"));
          p.setDescription(rs.getString("description"));
          p.setHdate(rs.getString("hdate"));
          rs.close();
          
          /*ps = con.prepareStatement("select hid, patient_id, doctor_id, heart_rate, body_temperature, glucose_level, blood_pressure, descriptioin, to_char(hdate, 'YYYY-MON-DD:HH24-MI-SS') hdate, from hparameters where patient_id = ? order by hdate desc");
          ps.setString(1,patient_id);
          rs = ps.executeQuery();
           ArrayList<HealthL> healthlist = new ArrayList<HealthL>();
          //Visit v;
          while ( rs.next()) {
               //healthlist.add(new HealthL(rs.getString("hid"), id, rs.getString("doctor_id"), rs.getString("heart_rate"),rs.getString("body_temperature"), rs.getString("glucose_level"), rs.getString("blood_pressure"), rs.getString("hdate")));
          }
          rs.close();
          p.setHealthl(healthlist); */
          return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getAllDetails()
    public static Patient getAllDDetails(String patient_id) {
      // connect and get all details of the given patient
      Connection con= null;
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select pd_name, pd_address, pd_contactno from patient_dependent where patient_id = ?");
          ps.setString(1,patient_id);
          ResultSet rs = ps.executeQuery();
          if (!rs.next () )
              return null;
          System.out.println("Patient Dependent Found");
          // create Patient object and load data from row to object
          Patient p = new Patient();
          p.setId(patient_id);
          p.setPname( rs.getString("pd_name"));
          p.setPaddress( rs.getString("pd_address"));
          p.setPcontactno( rs.getString("pd_contactno"));
          rs.close();
          return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getAllDetails()
    public static Patient getAllDocDetails(String patient_id) {
      // connect and get all details of the given patient
      Connection con= null;
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select doctor_id, d_name, specialization, d_address, d_contactno from doctorv where patient_id = ?");
          ps.setString(1,patient_id);
          ResultSet rs = ps.executeQuery();
          System.out.println(rs);
          System.out.println("pravahya doctor id error1 befror null");
          if (!rs.next () )
              return null;
          System.out.println("pravahya doctor id error2 after null");
          System.out.println("Doctor id Found");
          // create Patient object and load data from row to object
          Patient p = new Patient();
          p.setId(patient_id);
          p.setDoctorid(rs.getString("doctor_id"));
          p.setDname( rs.getString("d_name"));
          p.setSpecialization( rs.getString("specialization"));
          p.setDaddress( rs.getString("d_address"));
          p.setDcontactno(rs.getString("d_contactno"));
          rs.close();
          return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getAllDetails()
    
   
    
    public static Patient getDetails(String patient_id) {
      Connection con= null;
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select p_name, age, gender ,p_address, p_contactno from patients where patient_id = ?");
          ps.setString(1,patient_id);
          ResultSet rs = ps.executeQuery();
          if (! rs.next () )
              return null;
           Patient p = new Patient();
           p.setId(patient_id);
           p.setPname( rs.getString("p_name"));
           p.setAge( rs.getString("age"));
           p.setGender( rs.getString("gender"));
           p.setPaddress( rs.getString("p_address"));
           p.setPcontactno( rs.getString("p_contactno"));
          rs.close();
          return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getDetails()
    
    public static ArrayList<Patient> getPatients(String pname, String gender, String did) {
        String cond = " 1 = 1 ";
        if ( pname.length() > 0  )
            cond += " and upper(name) like '%" + pname.toUpperCase() + "%'";
        
      /*  if ( fromage.length() > 0 )
            cond += " and (sysdate - dob)/365 >= " + fromage;
        
        if ( toage.length() > 0 )
            cond += " and (sysdate - dob)/365 <= " + toage;
        */
        
        if (! gender.equals("a"))
            cond +=  " and gender = '" + gender + "'";
             // cond +=  " where gender = '" + gender + "'";
        Connection con = null;
        ArrayList<Patient> pl = new ArrayList<Patient>();
        try { 
            con = Database.getConnection();
            Statement st = con.createStatement();
            //ResultSet rs  = st.executeQuery("select p.pid,name, decode(gender,'m','Male','Female') gender, (sysdate-dob) / 365 age, complaints, to_char(visitdate) visitdate,contactno, referedby from patients p, visits v where p.mrvid = v.vid and " + cond);
            //ResultSet rs = st.executeQuery("select patient_id,p_name,age,decode(gender,'m','Male','Female') gender,p_address,p_contactno from patients" + cond);
            ResultSet rs = st.executeQuery("select distinct p.patient_id,p.p_name,p.age,p.gender,p.p_address,p.p_contactno from patients p, assigned_doctors a where p.patient_id=a.patient_id and a.doctor_id= " +did+ " and a.a_access='y';" );
            while ( rs.next()) {
                Patient p = new Patient();
                p.setId( rs.getString("patient_id"));
                p.setPname( rs.getString("p_name"));
                p.setAge( rs.getString("age"));
                p.setGender( rs.getString("gender"));
                p.setPaddress( rs.getString("p_address"));
                p.setPcontactno( rs.getString("p_contactno"));
                pl.add(p);
            } // while
            return pl;
        }
        catch(Exception ex) {
            System.out.println(ex.getMessage());
            return pl;
        }
        finally {
            Database.close(con);
        }
    } // getPatients
 public static ArrayList<Patient> getPatients1(String pid) {
     /*  String cond = " 1 = 1 ";
        if ( pname.length() > 0  )
            cond += " and upper(name) like '%" + pname.toUpperCase() + "%'";
        
        if ( fromage.length() > 0 )
            cond += " and (sysdate - dob)/365 >= " + fromage;
        
        if ( toage.length() > 0 )
            cond += " and (sysdate - dob)/365 <= " + toage;
        
        
        if (! gender.equals("a"))
            cond +=  " and gender = '" + gender + "'";
             // cond +=  " where gender = '" + gender + "'"; 
        */
        Connection con = null;
        ArrayList<Patient> pl = new ArrayList<Patient>();
        try { 
            con = Database.getConnection();
            Statement st = con.createStatement();  
            String query="select hid,hdate,bloodgroup,heart_rate,body_temperature,glucose_level,blood_pressure,description from hparameters where patient_id="+pid+" and access='y'";
            //ResultSet rs  = st.executeQuery("select p.pid,name, decode(gender,'m','Male','Female') gender, (sysdate-dob) / 365 age, complaints, to_char(visitdate) visitdate,contactno, referedby from patients p, visits v where p.mrvid = v.vid and " + cond);
            //ResultSet rs = st.executeQuery("select patient_id,p_name,age,decode(gender,'m','Male','Female') gender,p_address,p_contactno from patients" + cond);
            ResultSet rs = st.executeQuery(query);
            System.out.println(query);
            while ( rs.next()) {
                Patient p = new Patient();
                 p.setDaddress(rs.getString("hid"));
                 p.setHdate(rs.getString("hdate"));
                 p.setBgroup( rs.getString("bloodgroup"));
                 p.setHrate( rs.getString("heart_rate"));
                 p.setBtemp( rs.getString("body_temperature"));
                 p.setGlevel( rs.getString("glucose_level"));
                 p.setBpressure( rs.getString("blood_pressure"));
                 p.setDescription(rs.getString("description"));
                
                 pl.add(p);
            } // while
            return pl;
        }
        catch(Exception ex) {
            System.out.println(ex.getMessage());
            return pl;
        }
        finally {
            Database.close(con);
        }
    } // getPatients
   /*public String getMrvid() {
        return mrvid;
    }

    public void setMrvid(String mrvid) {
        this.mrvid = mrvid;
    }*/
    public static ArrayList<Patient> getPatientss(String pname, String gender) {
        String cond = " 1 = 1 ";
        if ( pname.length() > 0  )
            cond += " and upper(name) like '%" + pname.toUpperCase() + "%'";
        
      /*  if ( fromage.length() > 0 )
            cond += " and (sysdate - dob)/365 >= " + fromage;
        
        if ( toage.length() > 0 )
            cond += " and (sysdate - dob)/365 <= " + toage;
        */
        
        if (! gender.equals("a"))
            cond +=  " and gender = '" + gender + "'";
             // cond +=  " where gender = '" + gender + "'";
        Connection con = null;
        ArrayList<Patient> pl = new ArrayList<Patient>();
        try { 
            con = Database.getConnection();
            Statement st = con.createStatement();
            //ResultSet rs  = st.executeQuery("select p.pid,name, decode(gender,'m','Male','Female') gender, (sysdate-dob) / 365 age, complaints, to_char(visitdate) visitdate,contactno, referedby from patients p, visits v where p.mrvid = v.vid and " + cond);
            //ResultSet rs = st.executeQuery("select patient_id,p_name,age,decode(gender,'m','Male','Female') gender,p_address,p_contactno from patients" + cond);
            ResultSet rs = st.executeQuery("select patient_id,p_name,age,gender,p_address,p_contactno from patients");
            while ( rs.next()) {
                Patient p = new Patient();
                p.setId( rs.getString("patient_id"));
                p.setPname( rs.getString("p_name"));
                p.setAge( rs.getString("age"));
                p.setGender( rs.getString("gender"));
                p.setPaddress( rs.getString("p_address"));
                p.setPcontactno( rs.getString("p_contactno"));
                pl.add(p);
            } // while
            return pl;
        }
        catch(Exception ex) {
            System.out.println(ex.getMessage());
            return pl;
        }
        finally {
            Database.close(con);
        }
    } // getPatients
    
    
    public static ArrayList<Patient> getAssigneddcotors(String pid) {
    
        Connection con = null;
        ArrayList<Patient> pl = new ArrayList<Patient>();
        try { 
            con = Database.getConnection();
            Statement st = con.createStatement();
            //ResultSet rs  = st.executeQuery("select p.pid,name, decode(gender,'m','Male','Female') gender, (sysdate-dob) / 365 age, complaints, to_char(visitdate) visitdate,contactno, referedby from patients p, visits v where p.mrvid = v.vid and " + cond);
            //ResultSet rs = st.executeQuery("select patient_id,p_name,age,decode(gender,'m','Male','Female') gender,p_address,p_contactno from patients" + cond);
            ResultSet rs = st.executeQuery("select doctor_id,d_name from doctorv where patient_id="+pid);
            while ( rs.next()) {
                Patient p = new Patient();
                 p.setBpressure( rs.getString("doctor_id"));
                 p.setDescription(rs.getString("d_name"));
                 pl.add(p);
            } // while
            return pl;
        }
        catch(Exception ex) {
            System.out.println(ex.getMessage());
            return pl;
        }
        finally {
            Database.close(con);
        }
    } // getAssigneddoctors

   
     public static boolean update1(String str,String patient_id) {
         Connection con = null; 
      try {
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("update hparameters set access =? where patient_id = ?");
          ps.setString(1,str);
          ps.setString(2,patient_id);
          if ( ps.executeUpdate() == 1)
              return true;
          else
              return false; // error
      }
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return false;
      }
      finally {
          Database.close(con);
      }
   } // update1()
   
     
      public static boolean update2(String str,String patient_id,String doctor_id) {
         Connection con = null; 
      try {
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("update assigned_doctors set a_access =? where patient_id = ? and doctor_id = ?");
          ps.setString(1,str);
          ps.setString(2,patient_id);
          ps.setString(3,doctor_id);
          if ( ps.executeUpdate() == 1)
              return true;
          else
              return false; // error
      }
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return false;
      }
      finally {
          Database.close(con);
      }
   } // update1()
    
     
    public static ArrayList<Patient> getPatients2(String pid) {
     /*  String cond = " 1 = 1 ";
        if ( pname.length() > 0  )
            cond += " and upper(name) like '%" + pname.toUpperCase() + "%'";
        
        if ( fromage.length() > 0 )
            cond += " and (sysdate - dob)/365 >= " + fromage;
        
        if ( toage.length() > 0 )
            cond += " and (sysdate - dob)/365 <= " + toage;
        
        
        if (! gender.equals("a"))
            cond +=  " and gender = '" + gender + "'";
             // cond +=  " where gender = '" + gender + "'"; 
        */
        Connection con = null;
        ArrayList<Patient> pl = new ArrayList<Patient>();
        try { 
            con = Database.getConnection();
            Statement st = con.createStatement();
            //ResultSet rs  = st.executeQuery("select p.pid,name, decode(gender,'m','Male','Female') gender, (sysdate-dob) / 365 age, complaints, to_char(visitdate) visitdate,contactno, referedby from patients p, visits v where p.mrvid = v.vid and " + cond);
            //ResultSet rs = st.executeQuery("select patient_id,p_name,age,decode(gender,'m','Male','Female') gender,p_address,p_contactno from patients" + cond);
            ResultSet rs = st.executeQuery("select hdate,bloodgroup,heart_rate,body_temperature,glucose_level,blood_pressure,description from hparameters where patient_id="+pid);
            while ( rs.next()) {
                Patient p = new Patient();
                 p.setHdate(rs.getString("hdate"));
                 p.setBgroup( rs.getString("bloodgroup"));
                 p.setHrate( rs.getString("heart_rate"));
                 p.setBtemp( rs.getString("body_temperature"));
                 p.setGlevel( rs.getString("glucose_level"));
                 p.setBpressure( rs.getString("blood_pressure"));
                 p.setDescription(rs.getString("description"));
                
                 pl.add(p);
            } // while
            return pl;
        }
        catch(Exception ex) {
            System.out.println(ex.getMessage());
            return pl;
        }
        finally {
            Database.close(con);
        }
    } // getPatients

      
     public static Patient getAccessDetails(String pid,String ace){
      Connection con= null;
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select access from hparameters where patient_id="+pid);
          ps.setString(1,pid);
          ResultSet rs = ps.executeQuery();
          if (! rs.next () )
              return null;
           Patient p = new Patient();
           p.setId(pid);
           p.setPname( rs.getString("access"));
           rs.close();
          return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getDetails
     
     public static Patient getAccessDetails1(String patient_id) {
      // connect and get all details of the given patient
      Connection con= null;
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select a_access from assigned_doctors where patient_id = ?");
          ps.setString(1,patient_id);
          ResultSet rs = ps.executeQuery();
          if (!rs.next () )
              return null;
         // System.out.println("Patient Found");
          // create Patient object and load data from row to object
          Patient p = new Patient();
          p.setId(patient_id);
          p.setPname( rs.getString("a_access"));
          
          rs.close();
          return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getAllDetails()
    

     public static Patient getBloodgroup(String patient_id) {
      Connection con= null;
     
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select bloodgroup from hparameters where patient_id = ?");
          ps.setString(1,patient_id);
          ResultSet rs = ps.executeQuery();
          if (!rs.next () )
          {
              Patient p1 = new Patient();
              p1.setPname("n"); 
              rs.close();
                  return p1;
              //return null;
          } else {
           Patient p = new Patient();
           p.setId(patient_id);
           p.setPname( rs.getString("bloodgroup"));
           rs.close();
          return p;
          }
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getBloodgroup()
     
     
     
     
     
      public static Patient getHparametersDetails(String hid) {
      // connect and get all details of the given patient
      Connection con= null;
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select patient_id, bloodgroup, heart_rate, body_temperature, glucose_level, blood_pressure, description from hparameters where hid = ?");
          ps.setString(1,hid);
          ResultSet rs = ps.executeQuery();
          if(! rs.next())
              return null;
          // create Patient object and load data from row to object
          Patient p = new Patient();
     
          p.setPatientp(rs.getString("patient_id"));
          p.setBgroup( rs.getString("bloodgroup"));
          p.setHrate( rs.getString("heart_rate"));
          p.setBtemp( rs.getString("body_temperature"));
          p.setGlevel( rs.getString("glucose_level"));
          p.setBpressure( rs.getString("blood_pressure"));
          p.setDescription(rs.getString("description"));
         // p.setHdate(rs.getString("hdate"));
          rs.close();
          return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getAllDetails()
     
     
      public static boolean updateDescription(String hid, String description) {
         Connection con = null; 
      try {
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("update hparameters set description =? where hid = ?");
          ps.setString(1,description);
          ps.setString(2,hid);
        //  ps.setString(3,patient_id);
          if ( ps.executeUpdate() == 1)
              return true;
          else
              return false; // error
      }
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return false;
      }
      finally {
          Database.close(con);
      }
   } // update()
     
      
 public String referpatientadd(String assignedby,String assignedto,String PatientID1){
      Connection con = null; 
      try {
          con = Database.getConnection();
          Statement st = con.createStatement();
          //  p_access = "null";
          // get patient id
          ResultSet rs = st.executeQuery("select ifnull(max(rid),0) + 1 from recomendations");
          rs.next();
          id = rs.getString(1);
          rs.close();
          PreparedStatement ps = con.prepareStatement("insert into recomendations values(?,?,?,?)");
          ps.setString(1,id);
          ps.setString(2,assignedto);
          ps.setString(3,assignedby);
          ps.setString(4,PatientID1);
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
   } // referpatientadd()
 
 
  public static ArrayList<Patient> getRecommendations() {
    
        Connection con = null;
        ArrayList<Patient> pl = new ArrayList<Patient>();
        try { 
            con = Database.getConnection();
            Statement st = con.createStatement();
            //ResultSet rs  = st.executeQuery("select p.pid,name, decode(gender,'m','Male','Female') gender, (sysdate-dob) / 365 age, complaints, to_char(visitdate) visitdate,contactno, referedby from patients p, visits v where p.mrvid = v.vid and " + cond);
            //ResultSet rs = st.executeQuery("select patient_id,p_name,age,decode(gender,'m','Male','Female') gender,p_address,p_contactno from patients" + cond);
            ResultSet rs = st.executeQuery("select rid,ref_to_doctor_id,ref_by_doctor_id,patient_id from recomendations");
            while ( rs.next()) {
                Patient p = new Patient();
                 p.setBpressure( rs.getString("rid"));
                 p.setDescription(rs.getString("ref_to_doctor_id"));
                 p.setDaddress(rs.getString("ref_by_doctor_id"));
                 p.setBtemp(rs.getString("patient_id"));                 
                 pl.add(p);
            } // while
            return pl;
        }
        catch(Exception ex) {
            System.out.println(ex.getMessage());
            return pl;
        }
        finally {
            Database.close(con);
        }
    } // getAssigneddoctors
 
   public static Patient getRecDetails(String recid) {
      // connect and get all details of the given patient
      Connection con= null;
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select distinct doctor_id,d_name,specialization from doctors,recomendations where doctors.doctor_id=recomendations.ref_to_doctor_id and ref_to_doctor_id = ?");
          ps.setString(1,recid);
          ResultSet rs = ps.executeQuery();
          if(! rs.next())
              return null;
          // create Patient object and load data from row to object
          Patient p = new Patient();
     
          p.setPatientp(rs.getString("doctor_id"));
          p.setBgroup( rs.getString("d_name"));
          p.setHrate( rs.getString("specialization"));
          rs.close();
          return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getrecdocdetails()
 
   
     public static Patient getRecDetails1(String recid) {
      // connect and get all details of the given patient
      Connection con= null;
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select distinct doctor_id,d_name,specialization from doctors,recomendations where doctors.doctor_id=recomendations.ref_by_doctor_id and ref_by_doctor_id = ?");
          ps.setString(1,recid);
          ResultSet rs = ps.executeQuery();
          if(! rs.next())
              return null;
          // create Patient object and load data from row to object
          Patient p = new Patient();
     
          p.setPatientp(rs.getString("doctor_id"));
          p.setBgroup( rs.getString("d_name"));
          p.setHrate( rs.getString("specialization"));
          rs.close();
          return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getrecdocdetails1()
     
  
    public static Patient getRecDetails2(String recid) {
      // connect and get all details of the given patient
      Connection con= null;
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select distinct patient_id,p_name from patients where patient_id = ?");
          ps.setString(1,recid);
          ResultSet rs = ps.executeQuery();
          if(! rs.next())
              return null;
          // create Patient object and load data from row to object
          Patient p = new Patient();
     
          p.setPatientp(rs.getString("patient_id"));
          p.setBgroup( rs.getString("p_name"));
          rs.close();
          return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getrecdocdetails1()   

      public static Patient getRecDetails3(String recid) {
      // connect and get all details of the given patient
      Connection con= null;
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select distinct ref_to_doctor_id,patient_id from recomendations where rid = ?");
          ps.setString(1,recid);
          ResultSet rs = ps.executeQuery();
          if(! rs.next())
              return null;
          // create Patient object and load data from row to object
          Patient p = new Patient();
     
          p.setPatientp(rs.getString("ref_to_doctor_id"));
          p.setBgroup( rs.getString("patient_id"));
          rs.close();
          return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getrecdocdetails1() 
      
      
    public String addtoassigneddoctors(String patientid,String doctorid,String access){
      Connection con = null; 
      try {
          con = Database.getConnection();
          Statement st = con.createStatement();
          //  p_access = "null";
          // get patient id
          ResultSet rs = st.executeQuery("select max(aid)+1 from assigned_doctors");
          rs.next();
          id = rs.getString(1);
          rs.close();
          PreparedStatement ps = con.prepareStatement("insert into assigned_doctors values(?,?,?,?)");
          ps.setString(1,id);
          ps.setString(2,patientid);
          ps.setString(3,doctorid);
          ps.setString(4,access);
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
   } // referpatientadd()
    
    
     public static boolean delrec(String delID) {
        
         Connection con = null; 
      try {
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("delete from recomendations where rid = ?");
          ps.setString(1,delID);
          if ( ps.executeUpdate() == 1)
              return true;
          else
              return false; // error
      }
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return false;
      }
      finally {
          Database.close(con);
      }
   } // update
           
    public static ArrayList<Patient> getAssigneddcotorstable(String pid) {
    
        Connection con = null;
        ArrayList<Patient> pl = new ArrayList<Patient>();
        try { 
            con = Database.getConnection();
            Statement st = con.createStatement();
            //ResultSet rs  = st.executeQuery("select p.pid,name, decode(gender,'m','Male','Female') gender, (sysdate-dob) / 365 age, complaints, to_char(visitdate) visitdate,contactno, referedby from patients p, visits v where p.mrvid = v.vid and " + cond);
            //ResultSet rs = st.executeQuery("select patient_id,p_name,age,decode(gender,'m','Male','Female') gender,p_address,p_contactno from patients" + cond);
 ResultSet rs = st.executeQuery("select distinct doctors.doctor_id,doctors.d_name,doctors.specialization,assigned_doctors.a_access from doctors doctors,assigned_doctors assigned_doctors where doctors.doctor_id=assigned_doctors.doctor_id and assigned_doctors.patient_id="+pid);
            while ( rs.next()) {
                Patient p = new Patient();
                 p.setBpressure( rs.getString("doctor_id"));
                 p.setDescription(rs.getString("d_name"));
                 p.setBtemp(rs.getString("specialization"));
                 p.setBgroup(rs.getString("a_access"));
               
                 pl.add(p);
            } // while
            return pl;
        }
        catch(Exception ex) {
            System.out.println(ex.getMessage());
            return pl;
        }
        finally {
            Database.close(con);
        }
    } // getAssigneddoctors
    
     public static String checkrecords(String assby,String asto,String patient_id) {
      // connect and get all details of the given patient
      Connection con= null;
      String valueexist=""; 
      try{
          con = Database.getConnection();
          PreparedStatement ps = con.prepareStatement("select ref_by_doctor_id,ref_to_doctor_id,patient_id from recomendations where ref_by_doctor_id = ? and ref_to_doctor_id = ? and patient_id = ?");
          ps.setString(1,assby);
          ps.setString(2,asto);
          ps.setString(3,patient_id);
          ResultSet rs = ps.executeQuery();
          if (!rs.next ())
              return null;
          else
              rs.close();
              return valueexist;
                  
         // System.out.println("Patient Found");
         // create Patient object and load data from row to object
         //  Patient p = new Patient();
         //   p.setId(patient_id);
         //   p.setPname( rs.getString("a_access"));
         //   return p;
      } // end of try
      catch(Exception ex) {
          System.out.println(ex.getMessage());
          return null;
      }
      finally {
          Database.close(con);
      }
               
    } // getAllDetails()
} // Patient class


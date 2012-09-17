/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package mhealth;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
/**
 *
 * @author rkumar
 */
public class HealthL {
    private String hid,id,doctorid,bgroup,hrate,btemp,glevel,bpressure,description,hdate;
     public HealthL() {}
     public HealthL(String hid, String id, String doctorid,String hrate, String btemp, String glevel, String bpressure, String description, String hdate) {
           this.setHid(hid);
           this.id = id;
           this.doctorid =doctorid;
           this.hrate = hrate;
           this.btemp = btemp;
           this.glevel = glevel;
           this.bpressure= bpressure;
           this.description = description;
           this.hdate = hdate;
    } 
     
     public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
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
     public String getHid() {
        return hid;
    }

    public void setHid(String hid) {
        this.hid = hid;
    }
   public String getHdate() {
        return hdate;
    }

    public void setHdate(String hdate) {
        this.hdate = hdate;
    }
}

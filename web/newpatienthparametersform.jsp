<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<%@page import="java.sql.*"%> 
<%@page import="mhealth.*,java.util.*,java.lang.String"%>
<html>
    <head>
        <title>Patient Health parameters Form</title>
    </head>
    <body>
        <%@include file="head.html"%> 
        <% String pid1 = (String) session.getAttribute("patient_id"); 
        Patient par = Patient.getBloodgroup(pid1);
       // String value1=par;
        String value = par.getPname();
        System.out.println("Blooad group is:"+value); 
        %>
        <form action="newpatienthparameters.jsp" method="post">
          <table>
            
              <tr>
                  <td>Patient ID:</td>
                  <td><input type="text" name="pid" value="<%=pid1%>" readonly="readonly" size="5"></td> 
              </tr>

              <%  if(value.equalsIgnoreCase("n")){ %>
              <tr>
                  <td>Blood Group:</td>
                  <td><input type="text" name="bgroup" value="" size="5"></td> 
              </tr>
              <% }else{ %>
               <tr>
                  <td>Blood Group:</td>
                  <td><input type="text" name="bgroup" value="<%=value%>" readonly="readonly" size="5"></td> 
               </tr>
              <% } %>
              <tr>
                  <td> Heart rate: </td>
                  <td><input type="text" name="hrate"  value="" size="5"></td>
              </tr>
              <tr>
                  <td>Body temperature :</td>
                  <td><input type="text" name="btemp" value="" size="5" ></td>
              </tr>
              <tr>
                  <td>Glucose level :</td>
                  <td><input type="text" name="glevel" value="" size="5" ></td>
              </tr>
              <tr>
                  <td>Blood pressure :</td>
                  <td><input type="text" name="bpressure" value="" size="5" ></td>
              </tr>
              
          </table>
          <input type="submit" value="Add" />
          <input type="reset" value="Clear All" />
      </form>
    
    </body>
</html>

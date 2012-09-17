<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<%@page import="java.sql.*"%>
<html>
  <head>
    <title>New Patient Dependent Form</title>
  </head>
  <body>
      <%@include file="head.html"%>
      <h2>New Patient Dependent</h2>
      <form action="newpatientdependent.jsp" method="post">
          <table>
              <tr>
                  <td>Patient ID : </td>
                  <td>
    <select name="patientdidpf">
        <%
         Class.forName("com.mysql.jdbc.Driver");
        Connection con=DriverManager.getConnection("jdbc:mysql://localhost:3306/mhealthdefault","mhealth","mhealth");
         Statement st=con.createStatement(); 
           ResultSet rs=st.executeQuery("Select patient_id,p_name,p_contactno from patients");
        while(rs.next()){
    String str=rs.getString("patient_id");
    String str1=rs.getString("p_name");
    String str2=rs.getString("p_contactno");
%>
    <option value="<%=str%>"><%=str%>---<%=str1%>---<%=str2%></option>
    <%
}
%>
</select>
                  </td>
              </tr>
              <tr>
                  <td>Patient Relative Name:</td>
                  <td><input type="text" name="pdname"  size="50" /> 
              </tr>
              <tr>
                  <td>Relative Address : </td>
                  <td><textarea name="pdaddress" rows="4" cols="30"></textarea>
              </tr>
              <tr>
                  <td>Relative Contact :</td>
                  <td><input type="text" name="pdcontactno" value="" size="15" />
              </tr>
          </table>
          <p/>
          <input type="submit" value="Add PatientDependent" />
          <input type="reset" value="Clear All" />
      </form>
   <%@include file="footer1.html"%>
  </body>
</html>


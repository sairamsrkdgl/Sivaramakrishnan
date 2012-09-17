<%@page contentType="text/html" import="java.util.*,mhealth.*" %>
<%@page import="java.sql.*"%>
<%@page pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Doctor Details</title>
    </head>
    <body>
        <form action="addrefer.jsp" method="post">
    <%@include file="head.html"%> 
    <br><br><br><br><br><br><br><br>
    <table align="center">
                    
        <tr><td><font color="blue" size="3">Refer to doctor:</font></td>
             <td><select name="doctoridpf">
        <%
        String patient_id = request.getParameter("pid");
        session.setAttribute("patient_id", patient_id);
   
        Class.forName("com.mysql.jdbc.Driver");
        Connection con=DriverManager.getConnection("jdbc:mysql://localhost:3306/mhealthdefault","mhealth","mhealth");
        Statement st=con.createStatement(); 
        ResultSet rs=st.executeQuery("Select doctor_id,d_name,specialization from doctors where doctor_id not in (select doctor_id from assigned_doctors where patient_id="+patient_id+")");
        while(rs.next()){
         String str=rs.getString("doctor_id");
         String str1=rs.getString("d_name");
         String str2=rs.getString("specialization");
%>
    <option value="<%=str%>"><%=str%>---<%=str1%>---<%=str2%></option>
<%
}
%>
</select>
</td>
<td align="center"><input type="submit" value="Add" /></td>
     </tr>               
</table>

        </form>
        </body>
</html>

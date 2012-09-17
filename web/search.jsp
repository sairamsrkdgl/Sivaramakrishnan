<%@page contentType="text/html"%>
<%@page pageEncoding="UTF-8"%>
<%@page import="mhealth.*,java.util.*,java.lang.String"%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Search Patients</title>
    </head>
    <body>
    <%@include file="head.html"%>
    <h2>Search Patients</h2>
    <%
    out.print("Welcome:"+session.getAttribute("sUserName"));
    %>
    <form action="search.jsp">
         Name : <input type="text" size="10" name="pname_f" value="${param.pname}">
     <!--  Age : <input type="text" size="5" name="fromage" value="${param.fromage}">
        To <input type="text" size="5" name="toage" value="${param.toage}">  -->
        Gender :
        <select name="gender">
            <option value="a" ${param.gender =="a"? "SELECTED" :""}>All</option>    
            <option value="m" ${param.gender =="m"? "SELECTED" :""}>Male</option>    
            <option value="f" ${param.gender =="f"? "SELECTED" :""}>Female</option>                
        </select>
        <input type="submit" value="Search">
    </form>
    
    <%
     String did = (String) session.getAttribute("doctor_id");
     String pname = request.getParameter("pname_f");
     // do only when parameters are present
     if ( pname != null ) {
   //         String fromage = request.getParameter("fromage");
   //       String toage = request.getParameter("toage");
         String gender = request.getParameter("gender");
         ArrayList<Patient> patients = Patient.getPatients(pname,gender,did);
         if ( patients.size() == 0 )
             out.println("<h3>Sorry! No Patients Found!</h3>");
         else {
       %>
       <table border=1 width="100%">
          <tr>
          <th>Id </th>
          <th>Name</th>
          <!--<th>Gender </th>
          <th>Age </th>
          <th>Address</th> -->
          <th>Patient</th>
          <th>Doctor</th>
          <th>Health Parameters</th>
          <th>Patient Dependent</th>
       </tr>
       <%
         for ( Patient p : patients) {
              out.println( "<tr>");
              out.println("<td>" + p.getId() + "</td>");
              out.println("<td>" + p.getPname() + "</td>");
              //out.println("<td>" + p.getGender() + "</td>");
              //out.println("<td>" + p.getAge() + "</td>");
              //out.println("<td>" + p.getPaddress() + "</td>");
              out.println("<td><a href=getpatientdetails1.jsp?pid=" + p.getId() + ">click</a>" + "</td>");
              out.println("<td><a href=getdoctordetails.jsp?pid=" + p.getId() + ">click</a>" + "</td>");
              out.println("<td><a href=patientindex1.jsp?pid=" + p.getId() + ">click</a></td>");
              out.println("<td><a href=getpatientdepandent.jsp?pid=" + p.getId() + ">click</a></td>");
              out.println("</tr>");
         }
         out.println("</table>");
       }// else
      }
    %>
    <%@ include file="footer.html" %>
    </body>
</html>

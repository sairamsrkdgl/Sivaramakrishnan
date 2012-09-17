<%@page contentType="text/html"%>
<%@page pageEncoding="UTF-8"%>
<%@page import="mhealth.*,java.util.*,java.lang.String"%>
<%@page contentType="text/html" import="java.util.*,mhealth.*" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <titlePatient Details</title>
    </head>
    <body>
    <%@include file="head.html"%>
    <h2>Patient Details</h2>
    <%
     String mysession = (String) session.getAttribute("patient_id");   
    %> <div align="right"><a href="logout.jsp">Logout</a></div>    
    <% out.print("Welcome:"+mysession); %>
     <br/>
    <%
       mhealth.Patient p1 = mhealth.Patient.getAllDetails(mysession);
      %>
    Name:<%=p1.getPname()%><br/>
    Gender:<%=p1.getGender().equals("m") ? "Male" : "Female"%><br/>
    Address:<%=p1.getPaddress()%><br/>
    Contact No:<%=p1.getPcontactno()%><br/><br>
    
    <a href="newpatienthparametersform.jsp">Add Health parameters</a>
    
    <form action="">
     <!--    Name : <input type="text" size="10" name="pname_f" value="${param.pname}">
       Age : <input type="text" size="5" name="fromage" value="${param.fromage}">
        To <input type="text" size="5" name="toage" value="${param.toage}">  -->
       <!-- Gender :
        <select name="gender">
            <option value="a" ${param.gender =="a"? "SELECTED" :""}>All</option>    
            <option value="m" ${param.gender =="m"? "SELECTED" :""}>Male</option>    
            <option value="f" ${param.gender =="f"? "SELECTED" :""}>Female</option>                
        </select>
        <input type="submit" value="Search"> -->
    </form>
    
    <%
      String patientID = request.getParameter("pid");
      String pid = (String) session.getAttribute("patient_id");
     //String pname = request.getParameter("pname_f");
     // do only when parameters are present
    // if ( pname != null ) {
   //         String fromage = request.getParameter("fromage");
   //       String toage = request.getParameter("toage");
       //  String gender = request.getParameter("gender");
         ArrayList<Patient> patients = Patient.getPatients2(pid);
         if ( patients.size() == 0 )
             out.println("<h3>Sorry! No HealthParameter Records Found!</h3>");
         else {
       %>
       <br/>
       <h2>Health Parameters</h2>
       </br>
       <table border=1 width="100%">
          <tr>
          <th>Date</th>
          <th>Blood Group </th>
          <th>Heart Rate</th>
          <!--<th>Gender </th>
          <th>Age </th>
          <th>Address</th> -->
          <th>Body Temp</th>
          <th>Glucose </th>
          <th>Blood Pressure</th>
          <th>Description</th>
       </tr>
       <%
         for ( Patient p : patients) {
              out.println( "<tr>");
              out.println("<td>" + p.getHdate() + "</td>");
              out.println("<td>" + p.getBgroup() + "</td>");
              out.println("<td>" + p.getHrate() + "</td>");
              out.println("<td>" + p.getBtemp() + "</td>");
              out.println("<td>" + p.getGlevel() + "</td>");
              out.println("<td>" + p.getBpressure() + "</td>");
              out.println("<td>" + p.getDescription() + "</td>");
              
             
              out.println("</tr>");
         } //for
         out.println("</table>");
       }// else
      
    %>
    </br>
    <a href="settings1.jsp">View assigned doctor(s)</a></br</br></br>
    
   </body>
</html>
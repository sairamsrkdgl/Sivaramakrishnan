<%@page contentType="text/html" pageEncoding="windows-1252"%>
<%@page import="mhealth.*,java.util.*,java.lang.String"%>
<%@page contentType="text/html" import="java.util.*,mhealth.*" %>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
        <title>Patient assigned doctor(s)</title>
    </head>
    <body>
         <%@include file="head.html"%>
       
 <%   String patientID = request.getParameter("pid");
     // String pid = (String) session.getAttribute("patient_id");
      //Patient par = Patient.getAccessDetails1(pid);
      //String value = par.getPname();
      //System.out.println(value);    
      //String pname = request.getParameter("pname_f");
     // do only when parameters are present
    // if ( pname != null ) {
   //         String fromage = request.getParameter("fromage");
   //       String toage = request.getParameter("toage");
       //  String gender = request.getParameter("gender");
        
         ArrayList<Patient> patients = Patient.getAssigneddcotorstable(patientID);
         
         if ( patients.size() == 0 )
             out.println("<h3>Sorry! No Doctor is assigned!</h3>");
         else {
       %>
       <br/>
       <h3>Assigned Doctor(s)</h3></br>
       <div align="center"><table border=1 width="75%">
          <tr>
          <th>Doctor ID</th>
          <th>Doctor Name </th> 
          <th>Specialization </th>
          </tr>
       <%
         for ( Patient p : patients) {
              out.println( "<tr>");
              
              out.println("<td>" + p.getBpressure() + "</td>");
                  
              out.println("<td>" + p.getDescription() + "</td>");  
              
              out.println("<td>" + p.getBtemp() + "</td>");  
              
       
  
              out.println("</tr>");
         } //for
         out.println("</table>");
       }// else
      
    %>
       </div> 
       <a href="search1.jsp">Back</a>
    </body>
</html>
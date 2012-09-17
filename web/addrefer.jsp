
<%@page contentType="text/html" pageEncoding="windows-1252"%>
<%@page import="mhealth.*,java.util.*,java.lang.String"%>
<%@page contentType="text/html" import="java.util.*,mhealth.*" %>
<!DOCTYPE html>
<jsp:useBean class="mhealth.Patient" scope="page" id="patient" />
<jsp:setProperty name="patient" property="*" />
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
        <title>JSP Page</title>
    </head>
    <body>
         <%@include file="head.html"%>
         <br><br><br><br>
        <%
             String assignedby = (String) session.getAttribute("doctor_id");
             String assignedto = request.getParameter("doctoridpf");
             String patientID = (String) session.getAttribute("patient_id");
             
             String checkrecords = patient.checkrecords(assignedby,assignedto,patientID); 
             System.out.println("hai"+checkrecords);            
             if(checkrecords == null)
             {              
        //    if(checkrecords != null)                
             String result = patient.referpatientadd(assignedby,assignedto,patientID);
             if (result == null)
             out.println("Sorry! Could not refer to new doctor!");
             else
             out.println("New doctor is refered to patient successfully.");
             }
             else if(checkrecords != null)
             {
             out.println("Sorry! you have requested this doctor to admin. Admin approval is on process.");
             }  
     %>
     <br><br><br><br>
     <a href="/mhealth/search.jsp?pname_f=&gender=a">Back</a>
    </body>
</html>

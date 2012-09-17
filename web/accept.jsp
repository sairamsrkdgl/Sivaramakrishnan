
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
        <%
            
          //   String doctorid = (String) session.getAttribute("idr");       
          //   System.out.println(doctorid);            
          //   String patientid = (String) session.getAttribute("idrrr"); 
          //   System.out.println(patientid);   
           
        String previd = request.getParameter("pid"); 
   //     System.out.println(previd);  

        Patient part = Patient.getRecDetails3(previd);

        String idr = part.getPatientp();
   //     System.out.println(idr);  
     
        String idr1 = part.getBgroup();
  //      System.out.println(idr1);  

        String aa_access = "n";
        
        String result = patient.addtoassigneddoctors(idr1,idr,aa_access);
           
         boolean done = false;
         done = Patient.delrec(previd);
         
         if (result == null & done)
           
             out.println("Sorry! Could not assign doctor!");
        else
            out.println("New doctor is assigned to patient successfully.");
       %>
       <br>  
       <a href="/mhealth/viewrecomendations.jsp">Back</a>
    </body>
</html>

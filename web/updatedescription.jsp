
<%@page contentType="text/html" pageEncoding="windows-1252"%>
<%@page import="mhealth.*,java.util.*,java.lang.String"%>
<%@page contentType="text/html" import="java.util.*,mhealth.*" %>
<!DOCTYPE html>

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
        <title>JSP Page</title>
    </head>
    <body>
        <%
             String hparametersid = session.getAttribute("hid").toString();       
             String hdescription = request.getParameter("decription");
             String patientID = (String) session.getAttribute("patientID");
             boolean done = false;
                     done = Patient.updateDescription(hparametersid,hdescription); 
          
           if (! done )
              out.println("<h3>Could not update patient details! Try again!</h3>");
           else
           //   response.sendRedirect("/mhealth/editdescriptiondetails.jsp?hid="+hparametersid); 
                response.sendRedirect("/mhealth/patientindex1.jsp?pid="+patientID);       
       %>
    </body>
</html>

<%@page contentType="text/html"%>
<%@page pageEncoding="UTF-8" import="mhealth.*"%>


<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Update Patient Details</title>
    </head>
    <body>
    <%@ include file="head.html"%>        
    <h2>Update Patient</h2>

    <%
      String patient_id = session.getAttribute("pid").toString();
      // get details of Patient with the ID in Session object if it not postback
      String address = request.getParameter("paddress_f");
      String contactno = request.getParameter("pcontactno_f");
      String pname = request.getParameter("pname_f");
      boolean done = false;
      
      if ( address == null)  {
           Patient p = Patient.getDetails(patient_id);
           address = p.getPaddress();
           contactno =  p.getPcontactno();
           pname = p.getPname();
      }
      else {
           done = Patient.update(patient_id,contactno,address);
           if (! done )
              out.println("<h3>Could not update patient details! Try again!</h3>");
           else
              out.println("<h3>Patient Details Updated Successfull!</h3>");
     }
     if (! done ) {
%>         
    <form action="updatepatient.jsp" method="post">
        <table>
            <tr>
                <td>Patient Id </td>
                <td><%=patient_id %> </td>
            </tr>
            
            <tr>
                <td>Patient Name </td>
                <td><input type=text readonly size=20 name=pname_f value='<%= pname %>' </td>
            </tr>
            <tr>
                <td>Address </td>
                <td><textarea cols=30 name=paddress_f rows=3><%=address%></textarea> </td>
            </tr>
            
             <tr>
                <td>Contact No. </td>
                <td><input type=text size=20 name=pcontactno_f value='<%=contactno%>' </td>
            </tr>
        </table>
        <p/>
        <input type=submit value="Update" >
        <input type=reset value="Clear All">
    </form>
    <%
    }
    %>
    
    <%@ include file="footer1.html"%>             
    </body>
</html>

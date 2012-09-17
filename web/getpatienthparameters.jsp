<%@page contentType="text/html" import="java.util.*,mhealth.*" %>
<%@page pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Patient Health parameters Details</title>
    </head>
    <body>
        <%@include file="head.html"%>
    <h2>Patient Health Parameters </h2>
    <%
    String patient_id = request.getParameter("pid");
    // session.setAttribute("pid",patient_id);
    mhealth.Patient p = mhealth.Patient.getAllHDetails(patient_id);
    if ( p == null) {
         out.println("<h3>Sorry! Patient Health Parameters Information Could Not Be Retrieved!</h3>");
    }
    else {
    // display details of patient  
    %>
    <table >
        <tr>
            <td valign="center" >
                <table border=1 >
                    <tr>
                        <td>Blood Group</td>
                        <td><%= p.getBgroup()%></td>
                    </tr>          
                    <tr>
                        <td>Heart Rate(Per minute)</td>
                        <td><%= p.getHrate()%></td>
                    </tr>
                    <tr>
                        <td>Body temperature(Fahrenheit Scale) </td>
                        <td><%= p.getBtemp()%></td>
                    </tr>
                    <tr>
                        <td>Glucose Level(mmol/L)</td>
                        <td><%= p.getGlevel()%></td>
                    </tr>
                    <tr>
                        <td>Blood Pressure(mmHg) </td>
                        <td><%= p.getBpressure()%></td>
                    </tr>
                    <tr>
                        <td>Description </td>
                        <td><%= p.getDescription()%></td>
                    </tr>
                    <tr><td>Date</td>
                        <td><%= p.getHdate() %></td>
                    </tr>
                </table>
            </td>
          </tr>
       
                    <%
     }
     %> 
    </body>
</html>

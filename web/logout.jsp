<%-- 
    Document   : logout
    Created on : May 2, 2012, 4:25:43 PM
    Author     : rkumar
--%>

<%@page contentType="text/html" pageEncoding="windows-1252"%>
<!DOCTYPE html>
<%
  session.invalidate();
  response.sendRedirect("login.jsp");
%>
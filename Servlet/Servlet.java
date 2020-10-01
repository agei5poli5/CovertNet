package mypkg;
 
import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;
import java.util.*;
import java.util.stream.Collectors;
import java.net.URLDecoder;
import java.lang.String;
import java.lang.StringBuilder;
import java.util.ArrayList;
import java.net.*;
import javax.net.ssl.HttpsURLConnection;
import java.io.DataOutputStream;
import java.security.cert.Certificate;
public class Servlet extends HttpServlet {
   @Override
   public void doGet(HttpServletRequest request, HttpServletResponse response)
               throws IOException, ServletException {

      // Set the response message's MIME type
      response.setContentType("text/html;charset=UTF-8");
      // Allocate a output writer to write the response message into the network socket
      PrintWriter out = response.getWriter();
 
   }
   public void doPost(HttpServletRequest request, HttpServletResponse responsey)
            throws IOException, ServletException{
      
      responsey.setContentType("text/html;charset=UTF-8");
      PrintWriter out = responsey.getWriter();
      URLDecoder urld = new URLDecoder();
      String decode = "";
      String[] pairs;
      String[] i;
      ArrayList<String> headers = new ArrayList<String>();
      
    
      String[] temp = new String[2];


      int x;
      
                        
      // Write the response message, in an HTML page
      try {
      
       
       URL url = new URL("https://www.wikipedia.org");
      
       HttpsURLConnection connect = (HttpsURLConnection) url.openConnection();
       
      
         
            if ("POST".equalsIgnoreCase(request.getMethod())) 
      {
      
       decode = urld.decode(request.getReader().readLine());
       }

         
       pairs = decode.split("&");
       for(String s : pairs){
          i = s.split("=");
          //out.println(i[1]);
          headers.add(i[1]);
       }
       
       if(headers.get(0).matches("GET.*")){
         connect.setRequestMethod("GET");
       }
       else if(headers.get(0).matches("POST.*")){
         connect.setRequestMethod("POST");
       }
       else if(headers.get(0).matches("HEAD.*")){
         connect.setRequestMethod("HEAD");
       }
       else if(headers.get(0).matches("PUT*")){
         connect.setRequestMethod("PUT");
       }
       else if(headers.get(0).matches("CONNECT.*")){
         connect.setRequestMethod("CONNECT");
       }
       else if(headers.get(0).matches("OPTIONS.*")){
         connect.setRequestMethod("OPTIONS");
       }
       else if(headers.get(0).matches("DELETE.*")){
         connect.setRequestMethod("DELETE");
       }
       else if(headers.get(0).matches("PATCH.*")){
         connect.setRequestMethod("PATCH");
       }
      
       

       for(int k = 1; k < headers.size(); k++){
         temp = headers.get(k).split(":");
         out.print(temp[0]);
         out.println(temp[1]);
         /*if(temp[0].equalsIgnoreCase("Host")){
            url = new URL(temp[1]);
         }*/

         connect.setRequestProperty(temp[0], temp[1]);
         
         


       }
       
       
       
       
       connect.setUseCaches(false);
       connect.setDoOutput(true);
       connect.connect();
       BufferedReader br = new BufferedReader(new InputStreamReader((connect.getInputStream())));
       StringBuilder sb = new StringBuilder();
       String output;
       while ((output = br.readLine()) != null) {
       sb.append(output);
       }
       //out.println(sb.toString());
       StringBuilder resp = new StringBuilder();
       resp.append("xudtp=");
       resp.append(connect.getHeaderField(0));
       for(int j = 1; connect.getHeaderFieldKey(j) != null; j++){
         String key = "&xudtp=";
         resp.append(key);
         String h = String.format("%s:%s", connect.getHeaderFieldKey(j), connect.getHeaderField(j));
         resp.append(h);
       }
       resp.append("&ageispolisxudtp=");
       resp.append(sb.toString());
       out.println(resp);


      } finally {
         out.close();  // Always close the output writer
      }

   }
}
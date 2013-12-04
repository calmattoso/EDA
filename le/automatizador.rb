a= String.new()
#abx = File.new( "nomes", "w+")
Dir.foreach("abc"){|x|
	next if x=='.'||x==".."
	g=File.new "abc/#{x}", "r"
	a= x.concat("v2").to_s
    f= File.new  "abc/#{a}","w+"
    g.each_line do |line|

    	if (line[0]=='e'|| line[0]=='p')
    		f.puts("#{line.match(/[[:digit:]]+ +[[:digit:]]+/ )}")
    	end
    end

	#a = x.sub ".clq", ""
	#abx.puts("#{x.prepend("abc\\\\")}") # deveria ser a

}	

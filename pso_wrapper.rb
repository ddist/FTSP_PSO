#=== Deal with inputs.
if ARGV.length < 5
	puts "pso_wrapper.rb is a wrapper for the PSO algorithm."
	puts "Usage: ruby pso_wrapper.rb <instance_relname> <instance_specifics> <cutoff_time> <cutoff_length> <seed> <params to be passed on>."
	exit -1
end
inst_filename = ARGV[0]
instance_specifics = ARGV[1]
cutoff_time = ARGV[2].to_f
cutoff_length = ARGV[3].to_i
seed = ARGV[4].to_i

#=== Here I assume instance_specifics only contains the desired target quality or nothing at all for the instance, but it could contain more (to be specified in the instance_file or instance_seed_file)
if instance_specifics == ""
	inst_type = ""
else
	inst_type = instance_specifics.split[0]
end

paramstring = ARGV[5...ARGV.length].join(" ")

#=== Build algorithm command and execute it.
cmd = "./pso -seed #{seed} -inst #{inst_filename} -type #{inst_type} #{paramstring}"
puts cmd
filename = "./pso_output#{rand}.txt"
exec_cmd = "#{cmd} > #{filename}"

puts "Calling: #{exec_cmd}"
system exec_cmd

#=== Parse algorithm output to extract relevant information for ParamILS.
solved = nil
runtime = nil
runlength = nil
best_sol = nil

File.open(filename){|file|
	while line = file.gets
		if line =~ /-> Best Score = (.*)$/
			best_sol = -$1.to_f
			runlength = -$1.to_f
		end
		if line =~ /-> Runtime = (\d+)/
			runtime = $1.to_i
		end
	end
}

if best_sol == 0.0
	solved = "UNSAT"
else
	solved = "SAT"
end

File.delete(filename)
puts "Result for ParamILS: #{solved}, #{runtime}, #{runlength}, #{best_sol}, #{seed}"

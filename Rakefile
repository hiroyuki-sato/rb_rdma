require "bundler/gem_tasks"
task :default => :spec

require 'rake/extensiontask'

Rake::ExtensionTask.new('rb_rdma') do |ext|
  ext.lib_dir = "lib/rb_rdma"
end

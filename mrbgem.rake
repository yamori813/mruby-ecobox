MRuby::Gem::Specification.new('mruby-ecobox') do |spec|
  spec.license = 'BSDT'
  spec.authors = 'Hiroki Mori'

  if (`uname`.chomp =~ /freebsd/i)
    spec.linker.libraries << ['usb']
  else
    spec.linker.libraries << ['usb-1.0']
  end
end

# mruby-ecobox   [![Build Status](https://travis-ci.org/yamori813/mruby-ecobox.svg?branch=master)](https://travis-ci.org/yamori813/mruby-ecobox)
Ecobox class

This is IO Data ECOBOX-NET USB module mrbgem.


## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'yamori813/mruby-ecobox'
end
```
## example
```ruby
t = Ecobox.new
t.open

while 1 do
  if t.get(1) == 0 then
    p "I'm hier."
  end
end
```

## License
under the BSD License:
- see LICENSE file

## Todo

- cleanup

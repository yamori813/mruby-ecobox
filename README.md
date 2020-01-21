# mruby-ecobox   [![Build Status](https://travis-ci.org/yamori813/mruby-ecobox.svg?branch=master)](https://travis-ci.org/yamori813/mruby-ecobox)
Ecobox class

This is I-O DATA ECOBOX-NET USB module mrbgem.

get(timeout) - get event with timeout(millseconds). 0 is no timeout.
               return value is 0 is sccsess. -7 is timeout.
range(value) - set check range by ECOBOX_RANGE_30CM, 60CM, 90CM, 120CM.

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

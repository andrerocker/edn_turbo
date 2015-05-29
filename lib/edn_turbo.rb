require_relative 'edn_turbo/edn_parser'
require_relative 'edn_turbo/version'
require_relative 'edn_turbo/edn_turbo'

require 'date'
require 'set'
require 'edn'

module EDNT

  # ----------------------------------------------------------------------------
  # instantiate a parser (defined in the C-side) and parse the file
  #
  def self.read(input)

    data = input.instance_of?(String) ? input : input.read

    raise "EOF error" if data == ''

    Parser.new.read(data)

  end

  # ----------------------------------------------------------------------------
  # register a tagged element
  #
  TAGS = {}

  def self.register(tag, func = nil, &block)
    if block_given?
      func = block
    end

    if func.nil?
      func = lambda { |x| x }
    end

    if func.is_a?(Class)
      TAGS[tag] = lambda { |*args| func.new(*args) }
    else
      TAGS[tag] = func
    end
  end

  def self.unregister(tag)
    TAGS[tag] = nil
  end

  def self.tagged_element(tag, element)
    func = TAGS[tag]
    if func
      func.call(element)
    else
#      EDN::Type::Unknown.new(tag, element)
    end
  end

  # ----------------------------------------------------------------------------
  # handles creation of an rfc3339-formatted date based on date-formatted string
  #
  def self.rfc3339_date(str)
    d = nil
    begin
      d = DateTime.rfc3339(str)
    rescue ArgumentError
    end

    return d
  end

  # ----------------------------------------------------------------------------
  # handles creation of a set from an array
  #
  def self.make_set(elems)
    Set.new(elems)
  end

  # ----------------------------------------------------------------------------
  # handles creation of an EDN::Type::Symbol
  #
  def self.make_edn_symbol(elem)
    EDN::Type::Symbol.new(elem)
  end

end # EDN namespace

EDNT::register("inst") do |v|
  DateTime.parse(v)
end

EDNT::register("uuid") do |v|
  String.new(v)
end

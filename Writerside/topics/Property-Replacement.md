# Property Replacement
<link-summary>Use UPE to universally replace properties</link-summary>
<web-summary>Use UPE to universally replace properties</web-summary>
<card-summary>Use UPE to universally replace properties</card-summary>

## Feature

Allows you to globally modify any property with a new value.

## Usage

<include from="Snippets.topic" element-id="upe-api-folder"/>
<var name="upe-api-type" value="property replacement">
<var name="upe-api-folder" value="prop_overrides">

In that <path>.prop</path> file, add a string8s array, with each entry creating a new global property override.
```
string8s replacementList
end
```
To replace a property, add an entry to that list, each entry has the following format:
```
"data_type target_property_hash source_property_name"
```
`data_type` should be the value type of the target property you are replacing, for example, `float` or `int32`.

`target_property_hash` should be the numeric value of the property you are replacing, for example, to replace the property
`modelMinScale` you would use the number `0xF023ED73`. You can convert from a property name to the hash in the <ui-path>Utilities</ui-path> tab of SMFX.

Define a property with the `data_type` you put, and the name should match `source_property_name`.
This controls the value of the replaced property.

As an example, your file might look something like this:
```
string8s replacementList
	"float 0xF023ED73 minScaleValue"
	"float 0xF023ED79 maxScaleValue"
end
float maxScaleValue 340282346638528860000000000000000000000
float minScaleValue 0
```

<seealso style="cards">
    <category ref="external">
        <a href="https://zarklord.github.io/spore/Universal-Property-Enhancer.html" summary="Mod Page">Universal Property Enhancer</a>
    </category>
</seealso>
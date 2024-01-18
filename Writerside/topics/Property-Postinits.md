# Property Postinits
<link-summary>Use UPE to modify existing property lists</link-summary>
<web-summary>Use UPE to modify existing property lists</web-summary>
<card-summary>Use UPE to modify existing property lists</card-summary>

## Feature

Allows you to modify property lists, without actually modifying the original <path>.prop</path> file,
this is primarily useful in maintaining compatibility with other mods.

<tip>
    <p>
        Currently there is a limitation where you cannot add or remove individual elements from arrays.
    </p>
</tip>

## Usage

<include from="Snippets.topic" element-id="upe-api-folder"/>
<var name="upe-api-type" value="property postinit">
<var name="upe-api-folder" value="prop_postinits">

In that <path>.prop</path> file, add a string8s array, with each entry postinit-ing another <path>.prop</path> file.
```
string8s postinitList
end
```
To add a property list postinit, add an entry to that list, each entry has the following format:
```
"source_proplist target_proplist"
```
There should be a `key` property in the same file, with the same name that you used for `source_proplist`, which tells UPE what <path>.prop</path> file to postinit.

There should also be a `key` property in the same file, with the same name that you used for `target_proplist`, which tells UPE what <path>.prop</path> file to for postinit-ing.

As an example, your file might look something like this:
```
string8s postinitList
	"cl_mouth_filter_01 cl_mouth_filter_01_postinit"
	"cl_mouth_jaw_01 cl_mouth_jaw_01_postinit"
	"cl_mouth_poker_01 cl_mouth_poker_01_postinit"
end
key cl_mouth_filter_01 cell_parts~!cl_mouth_filter_01
key cl_mouth_filter_01_postinit my_postinits!cl_mouth_filter_01

key cl_mouth_jaw_01 cell_parts~!cl_mouth_jaw_01
key cl_mouth_jaw_01_postinit my_postinits!cl_mouth_jaw_01

key cl_mouth_poker_01 cell_parts~!cl_mouth_poker_01
key cl_mouth_poker_01_postinit my_postinits!cl_mouth_poker_01
```

Each postinit file can optionally contain these two special properties:
- ```
  key newParent path_to!new_parent
  ```
  Which will change the parent of the postinit-ed property.

- ```
  uint32s removeProperties
  end
  ```
  This is an array of property hashes, that will remove that property from the postinit-ed property.
  You can convert from a property name to the hash in the <ui-path>Utilities</ui-path> tab of SMFX.

  As an example, it might look like this:
  ```
  uint32s removeProperties
      0x9416CEA1 #modelHideDeformHandles
  end
  ```
  <tip>
      <p>
          It's a good idea to use a comment <code>#</code>, to put the actual property name after the number,
          so it's easier to read your files.
      </p>
  </tip>


<seealso style="cards">
    <category ref="external">
        <a href="https://zarklord.github.io/spore/Universal-Property-Enhancer.html" summary="Mod Page">Universal Property Enhancer</a>
    </category>
</seealso>
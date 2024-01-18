# Property List Duplication
<link-summary>Use UPE to duplicate property lists to new locations</link-summary>
<web-summary>Use UPE to duplicate property lists to new locations</web-summary>
<card-summary>Use UPE to duplicate property lists to new locations</card-summary>

## Feature

Allows you to make a single <path>.prop</path> get loaded in multiple places.
A common use case is for EditorKeys, where you commonly need to maintain the same exact file in multiple locations,
with this you can maintain one file and duplicate it to the other places.

## Usage

<include from="Snippets.topic" element-id="upe-api-folder"/>
<var name="upe-api-type" value="property duplications">
<var name="upe-api-folder" value="prop_duplications">

In that <path>.prop</path> file, add a string8s array, with each entry duplicating a <path>.prop</path> file.
```
string8s duplicateList
end
```
To duplicate a property, add an entry to that list, each entry has the following format:
```
"target_keyname source_keyname"
```
There should be a `key` property in the same file, with the same name that you used for `target_keyname`, which tells UPE where to duplicate this <path>.prop</path> file to.

There should also be a `key` property in the same file, with the same name that you used for `source_keyname`, which tells UPE what <path>.prop</path> file is being duplicated.

As an example, your file might look something like this:
```
string8s duplicateList
	"ca_editorkeys editor_keys"
	"ce_editorkeys editor_keys"
	"sa_editorkeys editor_keys"
	"ta_editorkeys editor_keys"
end
key ca_editorkeys ca_EditorKeys~!mycustom_editorkeys
key ce_editorkeys ce_EditorKeys~!mycustom_editorkeys
key sa_editorkeys sa_EditorKeys~!mycustom_editorkeys
key ta_editorkeys ta_EditorKeys~!mycustom_editorkeys

key editor_keys mycustom_editorkeys!editorkeys
```

<seealso style="cards">
    <category ref="external">
        <a href="https://zarklord.github.io/spore/Universal-Property-Enhancer.html" summary="Mod Page">Universal Property Enhancer</a>
    </category>
</seealso>
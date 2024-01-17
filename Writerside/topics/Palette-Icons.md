# Palette Icons
<link-summary>Use UPE to add custom palette icons to your part pages</link-summary>
<web-summary>Use UPE to add custom palette icons to your part pages</web-summary>
<card-summary>Use UPE to add custom palette icons to your part pages</card-summary>

All palette icon data is defined inside the <path>paletteicons</path> folder, so you must first create that folder in your mod.

Create a <path>.prop</path> file with a unique name in the <path>paletteicons</path> folder.
<warning>
    <p>
        Ensure that all prop files in the <path>paletteicons</path> folder have a unique name, if they share a name with another mod, it won't work.
    </p>
</warning>
<tip>
    <p>
        It's usually a good idea to use your mod's name, to reduce the chance of conflicting with other mods.
    </p>
</tip>

Add the following properties to that prop file:
<list type="bullet">
    <li>
        <code-block>
            key paletteSetButtonBackgroundIcon common~!bgicon.png
        </code-block>
        <p>
            Edit <code>common~!bgicon.png</code> to point to your icon background.
        </p>
    </li>
    <li>
        <code-block>
            key paletteSetButtonIcon common~!icon.png
        </code-block>
        <p>
            Edit <code>common~!icon.png</code> to point to your icon.
        </p>
    </li>
    <li>
        <code-block>
            key paletteSetButtonLayout layouts~!0x98F661C7.layout
        </code-block>
        <p>
            Usually, you don't need to edit this.
        </p>
    </li>
    <li>
        <code-block>
            texts paletteSetName
            &#x9;(0x0!0x0) ""
            end
        </code-block>
        <p>
            This is the text that appears when you hover, point it to your locale property and your text.
        </p>
    </li>
    <li>
        <code-block>
            int32 paletteSetSequenceNumber 1025
        </code-block>
        <p>
            This determines the ordering of palette icons in relation to each other
        </p>
        <warning>
            <p>
                This does not control the ordering of pages, so if this is set improperly,
                its possible for your icon to appear out of order compared to the pages it's on.
            </p>
        </warning>
    </li>
</list>

Then for every page you want to have your icon, add the following property:
<code-block>
int32 palettePageSetId hash(PALETTEICONPROP)
</code-block>
<p>
Edit <code>PALETTEICONPROP</code> to be the name of your palette icon prop file (excluding the file extension).
</p>